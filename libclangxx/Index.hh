/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/28/17.
//

#pragma once

#include "DeleteWith.hh"

#include <clang-c/Index.h>

#include <cassert>
#include <iosfwd>

namespace clx {

// forward declarations
class Cursor;
class Index;
class SourceLocation;
class TranslationUnit;
class Type;

class Index
{
public:
	Index();
	Index(Index&&) = default;
	
	TranslationUnit Parse(const std::string& filename, std::initializer_list<std::string> args, unsigned options);
	
	CXIndex Get();
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXIndex>,
		void,
		&::clang_disposeIndex
	> m_index;
};

class TranslationUnit
{
public:
	TranslationUnit(CXTranslationUnit tu);
	TranslationUnit(TranslationUnit&&) = default;
	
	std::string Spelling() const;
	Cursor Root();
	
	CXTranslationUnit Get();
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXTranslationUnit>,
		void,
		&::clang_disposeTranslationUnit
	> m_unit;
};

class Cursor
{
public:
	Cursor(CXCursor cursor);
	Cursor(const Cursor&) = default;
	Cursor(Cursor&&) = default;
	Cursor& operator=(const Cursor&) = default;
	Cursor& operator=(Cursor&&) = default;
	
	bool operator==(const Cursor& rhs) const;
	bool operator!=(const Cursor& rhs) const;
	
	CXCursorKind Kind() const;
	
	bool IsReference() const;
	bool IsDefinition() const;
	bool IsDeclaration() const;
	
	Cursor GetDefinition() const;
	
	std::string Spelling() const ;
	std::string DisplayName() const;
	std::string USR() const;
	
	SourceLocation Location() const;
	clx::Type Type() const;
	
	struct Hash
	{
		unsigned operator()(Cursor c) const;
	};
	
	template <typename Visitor>
	void Visit(Visitor visitor)
	{
		auto functor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult
		{
			Visitor *pv = reinterpret_cast<Visitor*>(client_data);
			
			assert(pv);
			(*pv)(Cursor{cursor}, Cursor{parent});
					
			return CXChildVisit_Continue;
		};
		
		::clang_visitChildren(m_cursor, functor, &visitor);
	}

private:
	CXCursor m_cursor;
};

class SourceLocation
{
public:
	SourceLocation(CXSourceLocation loc = {});
	
	void SpellingLocation(std::string& file, unsigned& line, unsigned& column, unsigned& offset) const;
	bool IsFromMainFile() const;
	bool IsFromSystemHeader() const;
	
private:
	CXSourceLocation m_loc;
};

std::ostream& operator<<(std::ostream& os, const SourceLocation& loc);

class Type
{
public:
	Type(CXType type);
	
	std::string Spelling() const;
	Cursor Declaration() const;

	friend std::ostream& operator<<(std::ostream& os, const Type& t);
	
private:
	CXType m_type;
};

} // end of namespace clx
