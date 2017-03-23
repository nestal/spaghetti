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
#include "EntityType.hh"

#include "ParentScopeImpl.hh"

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
			auto parent = dynamic_cast<DataType*>(FindByID(child.SemanticParent().USR()));
			if (parent)
				parent->VisitFunction(child);
		}
		break;
	}
	
	case CXCursor_Namespace:
		AddUnique<Namespace>(m_->cond, child.USR(), child, this).Visit(child);
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
