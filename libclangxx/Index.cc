/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/28/17.
//

#include "Index.hh"
#include "XStr.hh"

#include <vector>

namespace clx {

Index::Index() :
	m_index{::clang_createIndex(0, 0)}
{
	
}

TranslationUnit Index::Parse(const std::string& filename, std::initializer_list<std::string> args, unsigned options)
{
	std::vector<const char*> vargs;
	for (auto&& arg : args)
		vargs.push_back(arg.c_str());
	
	return {::clang_parseTranslationUnit(
		m_index.get(), filename.c_str(), &vargs[0], static_cast<int>(vargs.size()), 0, 0, options
	)};
}

CXIndex Index::Get()
{
	return m_index.get();
}

TranslationUnit::TranslationUnit(CXTranslationUnit tu) :
	m_unit{tu}
{
}

CXTranslationUnit TranslationUnit::Get()
{
	return m_unit.get();
}

std::string TranslationUnit::Spelling() const
{
	return XStr{::clang_getTranslationUnitSpelling(m_unit.get())}.Str();
}

Cursor TranslationUnit::Root()
{
	return Cursor{::clang_getTranslationUnitCursor(m_unit.get())};
}

Cursor::Cursor(CXCursor cursor) :
	m_cursor{cursor}
{
}

CXCursorKind Cursor::Kind() const
{
	return ::clang_getCursorKind(m_cursor);
}

std::string Cursor::DisplayName() const
{
	return XStr{::clang_getCursorDisplayName(m_cursor)}.Str();
}

std::string Cursor::USR() const
{
	return XStr{::clang_getCursorUSR(m_cursor)}.Str();
}

std::string Cursor::Spelling() const
{
	return XStr{::clang_getCursorSpelling(m_cursor)}.Str();
}

SourceLocation Cursor::Location() const
{
	return SourceLocation{::clang_getCursorLocation(m_cursor)};
}

bool Cursor::operator==(const Cursor& rhs) const
{
	return ::clang_equalCursors(m_cursor, rhs.m_cursor) != 0;
}

bool Cursor::operator!=(const Cursor& rhs) const
{
	return !operator==(rhs);
}

SourceLocation::SourceLocation(CXSourceLocation loc) :
	m_loc{loc}
{
}

void SourceLocation::SpellingLocation(std::string& file, unsigned& line, unsigned& column, unsigned& offset) const
{
	CXFile xfile;
	::clang_getSpellingLocation(m_loc, &xfile, &line, &column, &offset);
	file = XStr{::clang_getFileName(xfile)}.Str();
}

bool SourceLocation::IsFromMainFile() const
{
	return ::clang_Location_isFromMainFile(m_loc) != 0;
}

bool SourceLocation::IsFromSystemHeader() const
{
	return ::clang_Location_isInSystemHeader(m_loc) != 0;
}

unsigned Cursor::Hash::operator()(Cursor c) const
{
	return ::clang_hashCursor(c.m_cursor);
}
} // end of namespace
