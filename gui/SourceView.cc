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

#include "SendFunctorEvent.hh"

#include "libclx/Index.hh"
#include "libclx/SourceRange.hh"
#include "libclx/Token.hh"

#include <QtCore/QFile>

namespace gui {

SourceView::~SourceView()
{
	if (m_worker.joinable())
		m_worker.join();
}

void SourceView::Open(const libclx::SourceLocation& file)
{
	unsigned line, column, offset;
	file.Get(m_filename, line, column, offset);
	
	// set the default format before inserting text
	QTextCharFormat format;
	format.setForeground(QBrush{QColor{"black"}});
	format.setFontFamily("monospace");
	setCurrentCharFormat(format);
	
	QFile qfile{QString::fromStdString(m_filename)};
	if (qfile.open(QIODevice::ReadOnly))
		setPlainText(qfile.readAll());
	
	// only start 1 thread at a time
	if (m_worker.joinable())
		m_worker.join();
	
	// to improve latency, use a separate thread to parse the file
	m_worker = std::thread([this]{Parse();});
}

void SourceView::Parse()
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
		m_filename,
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
		
		if (token_fn == m_filename)
		{
			auto cit = text_colour.find(::clang_getTokenKind(token));
			auto stride = tstr.size();
			auto colour = cit->second;
			if (cit != text_colour.end())
				SendFunctorEvent(this, [this, line, column, stride, colour]
				{
					Highlight(line, column, stride, colour);
				});
		}
	}
}

void SourceView::Highlight(unsigned line, unsigned column, std::size_t stride, const QColor& colour)
{
	QTextCursor cursor(document());
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line-1);
	cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column-1);
	cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, static_cast<unsigned>(stride));
	
	setTextCursor(cursor);
	QTextCharFormat format;
	format.setForeground(QBrush{colour});
	format.setFontFamily("monospace");
	setCurrentCharFormat(format);
	
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	setTextCursor(cursor);
}

const std::string& SourceView::Filename() const
{
	return m_filename;
}
	
} // end of namespace
