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
		{
			auto it = FindByID(m_types, id);
			if (it == m_types.end())
			{
				m_types.push_back(Add<DataType>(cursor, this));
				it = --m_types.end();
			}
			(*it)->Visit(cursor);
			break;
		}
		
		case CXCursor_Namespace:
		{
			auto it = FindByID(m_ns, id);
			if (it == m_ns.end())
			{
				m_ns.push_back(Add<Namespace>(cursor, this));
				it = --m_ns.end();
			}
			(*it)->Visit(cursor);
			break;
		}
		
		case CXCursor_FieldDecl:
			m_vars.push_back(Add<Variable>(cursor, this));
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
	
} // end of namespace
