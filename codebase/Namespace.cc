/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "Namespace.hh"

#include "libclx/Cursor.hh"
#include <iostream>

namespace codebase {

Namespace::Namespace() :
	EntityVec{"Root", "", nullptr}
{
}

Namespace::Namespace(libclx::Cursor cursor, const Entity* parent) :
	EntityVec{cursor.Spelling(), cursor.USR(), parent}
{
}

std::string Namespace::Type() const
{
	return "Namespace";
}

void Namespace::Visit(libclx::Cursor self)
{
	self.Visit([this](libclx::Cursor cursor, libclx::Cursor)
	{
		if (cursor.Location().IsFromSystemHeader())
			return;
		
		const std::string& id = cursor.USR();
		switch (cursor.Kind())
		{
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl:
		case CXCursor_ClassTemplate:
			AddUnique(m_types, id, cursor, this)->Visit(cursor);
			break;
		
		case CXCursor_Namespace:
			AddUnique(m_ns, id, cursor, this)->Visit(cursor);
			break;
		
		case CXCursor_FieldDecl:
			AddUnique(m_vars, id, cursor, this);
			break;
			
		case CXCursor_CXXMethod:
			VisitMemberFunction(cursor);
			break;
		
		default:
//			if (!cursor.Location().IsFromSystemHeader())
//				std::cout << Name() << " " <<  cursor.Spelling() << ' ' << cursor.Kind() << std::endl;
			break;
		}
	});
}

void Namespace::CrossReference(EntityMap *)
{
}

void Namespace::VisitMemberFunction(libclx::Cursor cursor)
{
	// class method definition in namespace
	// the class definition should already be parsed
	auto parent = FindByID(m_types, cursor.SemanticParent().USR());
	
	if (parent != m_types.end())
		(*parent)->VisitFunction(cursor);
}

void Namespace::MarkUsed()
{
	MarkSelfUsedOnly();
}

void Namespace::Swap(Namespace& other)
{
	EntityVec::Swap(other);
	
	using std::swap;
	swap(m_ns, other.m_ns);
	swap(m_types, other.m_types);
	swap(m_vars, other.m_vars);
}
	
} // end of namespace
