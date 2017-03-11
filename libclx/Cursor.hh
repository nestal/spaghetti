/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#pragma once

#include <clang-c/Index.h>
#include <string>

#include <cassert>
#include <iostream>

namespace libclx {

class SourceLocation;
class SourceRange;
class Type;

class Cursor
{
public:
	Cursor();
	Cursor(CXCursor cursor);
	Cursor(const Cursor&) = default;
	Cursor(Cursor&&) = default;
	Cursor& operator=(const Cursor&) = default;
	Cursor& operator=(Cursor&&) = default;
	
	bool operator==(const Cursor& rhs) const;
	bool operator!=(const Cursor& rhs) const;
	
	CXCursorKind Kind() const;
	
	explicit operator bool() const;
	
	bool IsReference() const;
	bool IsDefinition() const;
	bool IsDeclaration() const;
	
	Cursor GetDefinition() const;
	Cursor SemanticParent() const;
	Cursor LexicalParent() const;
	Cursor Referenced() const;
	
	std::string Spelling() const ;
	std::string DisplayName() const;
	std::string USR() const;
	std::string Comment() const;
	
	SourceLocation Location() const;
	SourceRange Extent() const;
	libclx::Type Type() const;
	libclx::Type ResultType() const;
	
	struct Hash
	{
		unsigned operator()(Cursor c) const;
	};
	
	template <typename Visitor>
	void Visit(Visitor visitor, bool recurse = false)
	{
		struct ClientData
		{
			Visitor *visitor;
			bool recurse;
		} data {&visitor, recurse};
		
		auto functor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult
		{
			Visitor *pv  = reinterpret_cast<ClientData*>(client_data)->visitor;
			bool recurse = reinterpret_cast<ClientData*>(client_data)->recurse;
			
			assert(pv);
			(*pv)(Cursor{cursor}, Cursor{parent});
			
			return recurse ? CXChildVisit_Recurse : CXChildVisit_Continue;
		};

		::clang_visitChildren(m_cursor, functor, &data);
	}

private:
	CXCursor m_cursor;
};

} // end of namespace
