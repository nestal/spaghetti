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

#include "DataType.hh"

#include "libclx/Cursor.hh"

namespace codebase {

Namespace::Namespace() :
	ParentScope{"Root", "", nullptr}
{
}

Namespace::Namespace(const libclx::Cursor& cursor, const Entity* parent) :
	ParentScope{cursor, parent}
{
}

std::string Namespace::Type() const
{
	return "Namespace";
}

void Namespace::VisitChild(const libclx::Cursor& child, const libclx::Cursor& self)
{
	switch(child.Kind())
	{
	case CXCursor_CXXMethod:
	{
		if (child.SemanticParent() == self)
			ParentScope::VisitChild(child, self);
		
		else
		{
			// class method definition in namespace
			// the class definition should already be parsed
			auto parent = FindByID(Types(), child.SemanticParent().USR());
			
			if (parent != Types().end())
				(*parent)->VisitFunction(child);
		}
		break;
	}
	
	case CXCursor_Namespace:
		AddUnique(m_ns, child.USR(), child, this)->Visit(child);
		break;
	
	default:
		ParentScope::VisitChild(child, self);
		break;
	}
}

void Namespace::CrossReference(EntityMap *)
{
}

void Namespace::MarkUsed()
{
	MarkSelfUsedOnly();
}

} // end of namespace
