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

namespace clx {

class TranslationUnit;

class Index
{
public:
	Index();
	Index(Index&&) = default;
	
	TranslationUnit Parse(std::initializer_list<std::string> args, unsigned options);
	
	CXIndex Get();
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXIndex>,
		void,
		&::clang_disposeIndex
	> m_index;
};

class Cursor;

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
	
	CXCursorKind Kind() const;
	
	std::string Spelling() const ;
	std::string DisplayName() const;
	std::string USR() const;
	
	template <typename Visitor>
	void Visit(Visitor visitor)
	{
		auto functor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult
		{
			Visitor *visitor = reinterpret_cast<Visitor*>(client_data);
			
			Cursor current{cursor};
			(*visitor)(current, Cursor{parent});
			current.Visit(*visitor);
						
			return CXChildVisit_Continue;
		};
		
		::clang_visitChildren(m_cursor, functor, &visitor);
	}

private:
	CXCursor m_cursor;
};

} // end of namespace clx
