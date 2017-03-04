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

#include "gui/common/SendFunctorEvent.hh"

#include "libclx/Index.hh"
#include "libclx/SourceRange.hh"
#include "libclx/Token.hh"

#include <QtCore/QFile>

namespace gui {
namespace sourcevw {

SourceView::SourceView(SourceModel *model, QWidget *parent) :
	QPlainTextEdit{parent},
	m_model{model},
	m_highlight{document()}
{
	connect(m_model, &SourceModel::OnLocationChanged, [this]{
		Open(m_model->Name(), m_model->Line(), m_model->Column());
	});
}

SourceView::~SourceView()
{
	if (m_worker.joinable())
		m_worker.join();
}

void SourceView::Open(const std::string& fname, unsigned line, unsigned column)
{
	if (fname != m_filename)
	{
		m_filename = fname;
		
		setUndoRedoEnabled(false);
		
		// set the default format before inserting text
		QTextCharFormat format;
		format.setForeground(QBrush{QColor{"black"}});
		format.setFontFamily("monospace");
		setCurrentCharFormat(format);
		
		// only start 1 thread at a time
		if (m_worker.joinable())
			m_worker.join();
		
		// to improve latency, use a separate thread to parse the file
		m_worker = std::thread([this, line, column]{Parse(line, column);});
	}
	else
		GoTo(line, column);
}

void SourceView::Parse(unsigned line, unsigned column)
{
	QFile qfile{QString::fromStdString(m_filename)};
	if (qfile.open(QIODevice::ReadOnly))
	{
		auto all = qfile.readAll();
		SendFunctorEvent(this, [this, all]
		{
			setPlainText(all);
			m_highlight.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		}, Qt::LowEventPriority);
	}
		
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
		CXTranslationUnit_Incomplete
	);
	
	for (auto&& token: tu.Tokenize())
	{
		auto tloc = tu.TokenLocation(token);
		auto tstr = tu.TokenSpelling(token);
		
		std::string token_fn;
		unsigned tline, tcolumn, toffset;
		tloc.Get(token_fn, tline, tcolumn, toffset);
		
		if (token_fn == m_filename)
		{
			auto cit = text_colour.find(::clang_getTokenKind(token));
			auto stride = tstr.size();
			auto colour = cit->second;
			if (cit != text_colour.end())
			{
				SendFunctorEvent(this, [this, tline, tcolumn, stride, colour]
				{
					Highlight(tline, tcolumn, stride, colour);
				}, Qt::LowEventPriority);
			}
		}
	}
	
	SendFunctorEvent(this, [this, line, column]
	{
		GoTo(line, column);
	}, Qt::LowEventPriority);
}

void SourceView::Highlight(unsigned line, unsigned column, std::size_t stride, const QColor& colour)
{
	int block = m_highlight.blockNumber();
	
	m_highlight.movePosition(QTextCursor::StartOfLine,     QTextCursor::MoveAnchor);
	m_highlight.movePosition(QTextCursor::Down,            QTextCursor::MoveAnchor, line-1   - block);
	m_highlight.movePosition(QTextCursor::Right,           QTextCursor::MoveAnchor, column-1);
	m_highlight.movePosition(QTextCursor::Right,           QTextCursor::KeepAnchor, static_cast<unsigned>(stride));
	
	QTextCharFormat format;
	format.setForeground(QBrush{colour});
	m_highlight.mergeCharFormat(format);
}

const std::string& SourceView::Filename() const
{
	return m_filename;
}

void SourceView::GoTo(unsigned line, unsigned column)
{
	QTextCursor cursor{document()};
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line-1);
	cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column-1);
	setTextCursor(cursor);
}

SourceModel* SourceView::Model()
{
	return m_model;
}

QWidget *SourceView::Widget()
{
	return this;
}

void SourceView::ResetZoom()
{
}
	
}} // end of namespace
