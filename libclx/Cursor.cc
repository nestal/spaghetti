/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include <clang-c/Index.h>
#include "Cursor.hh"

#include "SourceRange.hh"
#include "Type.hh"
#include "XStr.hh"

namespace libclx {

Cursor::Cursor() :
	Cursor{::clang_getNullCursor()}
{
}

Cursor::Cursor(CXCursor cursor) :
	m_cursor{cursor}
{
}

CXCursorKind Cursor::Kind() const
{
	return ::clang_getCursorKind(m_cursor);
}

std::string Cursor::KindSpelling() const
{
	return XStr{::clang_getCursorKindSpelling(Kind())}.Str();
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
	return ::clang_isReference(Kind()) != 0;
}

bool Cursor::IsDefinition() const
{
	return ::clang_isCursorDefinition(m_cursor) != 0;
}

Cursor Cursor::GetDefinition() const
{
	return {::clang_getCursorDefinition(m_cursor)};
}

Cursor Cursor::SemanticParent() const
{
	return {::clang_getCursorSemanticParent(m_cursor)};
}

Cursor Cursor::LexicalParent() const
{
	return {::clang_getCursorLexicalParent(m_cursor)};
}

bool Cursor::IsDeclaration() const
{
	return ::clang_isDeclaration(Kind()) != 0 ;
}

std::string Cursor::Comment() const
{
	return XStr{::clang_Cursor_getRawCommentText(m_cursor)}.Str();
}

SourceRange Cursor::Extent() const
{
	return SourceRange{::clang_getCursorExtent(m_cursor)};
}

Cursor::operator bool() const
{
	return ::clang_equalCursors(m_cursor, ::clang_getNullCursor()) == 0;
}

libclx::Type Cursor::ResultType() const
{
	return {::clang_getCursorResultType(m_cursor)};
}

Cursor Cursor::Referenced() const
{
	return {::clang_getCursorReferenced(m_cursor)};
}

Cursor Cursor::SpecializedCursorTemplate() const
{
	return {::clang_getSpecializedCursorTemplate(m_cursor)};
}

unsigned Cursor::Hash::operator()(Cursor c) const
{
	return ::clang_hashCursor(c.m_cursor);
}

} // end of namespace
