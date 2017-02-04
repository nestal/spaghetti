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
#include <ostream>
#include <clang-c/Index.h>

namespace libclx {

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

TranslationUnit::TranslationUnit(CXTranslationUnit tu) :
	m_unit{tu}
{
}

std::string TranslationUnit::Spelling() const
{
	return XStr{::clang_getTranslationUnitSpelling(m_unit.get())}.Str();
}

Cursor TranslationUnit::Root()
{
	return Cursor{::clang_getTranslationUnitCursor(m_unit.get())};
}

boost::iterator_range<TranslationUnit::diag_iterator> TranslationUnit::Diagnostics() const
{
	return {
		diag_iterator{0, m_unit.get()},
		diag_iterator{::clang_getNumDiagnostics(m_unit.get()), m_unit.get()}
	};
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

libclx::Type Cursor::Type() const
{
	return {::clang_getCursorType(m_cursor)};
}

bool Cursor::IsReference() const
{
	return ::clang_isCursorDefinition(m_cursor) != 0;
}

bool Cursor::IsDefinition() const
{
	return ::clang_isCursorDefinition(m_cursor) != 0;
}

Cursor Cursor::GetDefinition() const
{
	return {::clang_getCursorDefinition(m_cursor)};
}

bool Cursor::IsDeclaration() const
{
	return ::clang_isDeclaration(Kind()) != 0 ;
}

std::string Cursor::Comment() const
{
	return XStr{::clang_Cursor_getRawCommentText(m_cursor)}.Str();
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

std::ostream& operator<<(std::ostream& os, const SourceLocation& loc)
{
	std::string file;
	unsigned line, column, offset;
	loc.SpellingLocation(file, line, column, offset);
	
	return os << file << ":" << line ;
}

unsigned Cursor::Hash::operator()(Cursor c) const
{
	return ::clang_hashCursor(c.m_cursor);
}

Type::Type(CXType type) :
	m_type{type}
{
}

std::string Type::Spelling() const
{
	return XStr{::clang_getTypeSpelling(m_type)}.Str();
}

std::ostream& operator<<(std::ostream& os, const Type& t)
{
	return os << t.Spelling() << ": " << " (" << t.Declaration().USR() << ")";
}

Cursor Type::Declaration() const
{
	return {::clang_getTypeDeclaration(m_type)};
}

std::string Type::Kind() const
{
	return XStr{::clang_getTypeKindSpelling(m_type.kind)}.Str();
}

void TranslationUnit::diag_iterator::increment()
{
	m_idx++;
}

bool TranslationUnit::diag_iterator::equal(const TranslationUnit::diag_iterator& other) const
{
	return m_idx == other.m_idx && m_parent == other.m_parent;
}

Diagnostic TranslationUnit::diag_iterator::dereference() const
{
	return Diagnostic{::clang_getDiagnostic(m_parent, m_idx)};
}

std::string Diagnostic::Str() const
{
	return XStr{::clang_formatDiagnostic(m_diag.get(), ::clang_defaultDiagnosticDisplayOptions())}.Str();
}
} // end of namespace
