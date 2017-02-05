/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "SourceView.hh"

#include "libclx/Index.hh"
#include "libclx/SourceRange.hh"
#include "libclx/Token.hh"

#include <QtCore/QFile>
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>

namespace gui {

class SourceView::HighlightEvent : public QEvent
{
public:
	HighlightEvent(unsigned line, unsigned column, std::size_t stride, const QColor& colour) :
		QEvent{static_cast<QEvent::Type>(TypeEnum())},
		m_line{line},
		m_column{column},
		m_stride{stride},
		m_colour{colour}
	{
	}
	
	static int TypeEnum()
	{
		static const int type = QEvent::registerEventType();
		return type;
	}
	
	void Highlight(QTextEdit& edit)
	{
		QTextCursor cursor(edit.document());
		cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, m_line-1);
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, m_column-1);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, static_cast<unsigned>(m_stride));
		
		edit.setTextCursor(cursor);
		QTextCharFormat format;
		format.setForeground(QBrush{m_colour});
		format.setFontFamily("monospace");
		edit.setCurrentCharFormat(format);
		
		cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		edit.setTextCursor(cursor);
	}
	
private:
	unsigned m_line, m_column;
	std::size_t m_stride;
	QColor m_colour;
};

SourceView::~SourceView()
{
	if (m_worker.joinable())
		m_worker.join();
}

void SourceView::Open(const libclx::SourceLocation& file)
{
	std::string filename;
	unsigned line, column, offset;
	file.Get(filename, line, column, offset);
	
	// set the default format before inserting text
	QTextCharFormat format;
	format.setForeground(QBrush{QColor{"black"}});
	format.setFontFamily("monospace");
	setCurrentCharFormat(format);
	
	QFile qfile{QString::fromStdString(filename)};
	if (qfile.open(QIODevice::ReadOnly))
		setPlainText(qfile.readAll());
	
	// only start 1 thread at a time
	if (m_worker.joinable())
		m_worker.join();
	
	// to improve latency, use a separate thread to parse the file
	m_worker = std::thread([this, filename]{ Parse(filename);});
}

void SourceView::Parse(const std::string& filename)
{
	static const std::map<CXTokenKind, QColor> text_colour = {
		{CXToken_Punctuation, QColor{"black"}},
		{CXToken_Keyword, QColor{"blue"}},
		{CXToken_Comment, QColor{"green"}},
		{CXToken_Identifier, QColor{"black"}},
		{CXToken_Literal, QColor{"red"}},
	};
	
	libclx::Index clx;
	auto tu = clx.Parse(
		filename,
		{
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", ".",
		},
		CXTranslationUnit_None
	);
	
	for (auto&& token: tu.Tokenize())
	{
		auto tloc = tu.TokenLocation(token);
		auto tstr = tu.TokenSpelling(token);
		
		std::string token_fn;
		unsigned line, column, offset;
		tloc.Get(token_fn, line, column, offset);
		
		if (token_fn == filename)
		{
			auto cit = text_colour.find(::clang_getTokenKind(token));
			if (cit != text_colour.end())
				QCoreApplication::postEvent(this, new HighlightEvent{line, column, tstr.size(), cit->second});
		}
	}
}

bool SourceView::event(QEvent *ev)
{
	if (ev->type() == HighlightEvent::TypeEnum())
	{
		auto highlight = static_cast<HighlightEvent*>(ev);
		highlight->Highlight(*this);
		return true;
	}
	
	return QTextEdit::event(ev);
}
	
} // end of namespace
