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
#include "ClassTemplate.hh"
#include "EntityType.hh"

#include "libclx/Cursor.hh"

namespace codebase {

Namespace::Namespace() :
	ParentScope{"Root", "", nullptr}
{
}

Namespace::Namespace(const libclx::Cursor& cursor, const EntityVec* parent) :
	ParentScope{cursor, parent}
{
}

EntityType Namespace::Type() const
{
	return EntityType::namespace_;
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
			if (auto parent = FindDataType(child.SemanticParent().USR()))
				parent->VisitFunction(child);
			else if (auto temp = FindClassTemplate(child.SemanticParent().USR()))
				temp->VisitFunction(child);
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


} // end of namespace
