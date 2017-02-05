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
#include <iostream>

namespace gui {

void SourceView::Open(const libclx::SourceLocation& file)
{
	std::string filename;
	unsigned line, column, offset;
	file.Get(filename, line, column, offset);
	
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
	
	QFile qfile{QString::fromStdString(filename)};
	if (qfile.open(QIODevice::ReadOnly))
		setText(qfile.readAll());
	
	static const std::map<CXTokenKind, QColor> text_colour = {
		{CXToken_Punctuation, QColor{"black"}},
		{CXToken_Keyword,     QColor{"blue"}},
		{CXToken_Comment,     QColor{"green"}},
		{CXToken_Identifier,  QColor{"purple"}},
		{CXToken_Literal,     QColor{"red"}},
	};
	
	for (auto&& token: tu.Tokenize())
	{
		auto tloc = tu.TokenLocation(token);
		auto tstr = tu.TokenSpelling(token);
				
		std::string token_fn;
		tloc.Get(token_fn, line, column, offset);
		
		if (token_fn == filename)
		{
			auto cit = text_colour.find(::clang_getTokenKind(token));
			if (cit != text_colour.end())
				Highlight(line, column, tstr.size(), cit->second);
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
}
	
} // end of namespace
