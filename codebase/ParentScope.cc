/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#include "ParentScope.hh"

#include "EntityType.hh"
#include "Function.hh"
#include "Variable.hh"
#include "ClassTemplate.hh"
#include "TypeAlias.hh"

#include "libclx/Cursor.hh"


#include <ostream>
#include <iostream>

namespace codebase {

ParentScope::ParentScope(const libclx::Cursor& cursor, const EntityVec* parent) :
	EntityVec{cursor.DisplayName(), cursor.USR(), parent}
{
}

ParentScope::ParentScope(const std::string& name, const std::string& usr, const EntityVec *parent) :
	EntityVec{name, usr, parent}
{
}

void ParentScope::Visit(const libclx::Cursor& self)
{
	OnVisit(self);
	self.Visit([this](auto child, auto parent){this->VisitChild(child, parent);});
}

void ParentScope::VisitChild(const libclx::Cursor& child, const libclx::Cursor&)
{
	switch (child.Kind())
	{
	case CXCursor_FieldDecl:
		AddUnique(m_fields, child.USR(), child, this);
		break;
		
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
		AddUnique(m_types, child.USR(), child, this)->Visit(child);
		break;
	
	case CXCursor_ClassTemplate:
		AddUnique(m_temps, child.USR(), child, this)->Visit(child);
		break;
	
	case CXCursor_CXXMethod:
		AddUnique(m_func, child.USR(), child, this)->Visit(child);
		break;
	
	case CXCursor_TypeAliasDecl:
		AddUnique(m_alias, child.USR(), child, this);
		break;
		
	default:
		break;
	}
}

boost::iterator_range<ParentScope::field_iterator> ParentScope::Fields() const
{
	return {m_fields.begin(), m_fields.end()};
}

boost::iterator_range<ParentScope::function_iterator> ParentScope::Functions() const
{
	return {m_func.begin(), m_func.end()};
}

boost::iterator_range<ParentScope::class_template_iterator> ParentScope::ClassTemplates() const
{
	return {m_temps.begin(), m_temps.end()};
}

boost::iterator_range<ParentScope::data_type_iterator> ParentScope::DataTypes() const
{
	return {m_types.begin(), m_types.end()};
}

void ParentScope::OnVisit(const libclx::Cursor&)
{
}

void ParentScope::VisitFunction(const libclx::Cursor& func)
{
	auto func_entity = FindInVec(m_func, func.USR());
	
	// the definition of a member function should come after its declaration
	if (func_entity)
		func_entity->Visit(func);
}

DataType& ParentScope::Add(std::unique_ptr<DataType>&& inst)
{
	assert(inst->Parent() == this);
	EntityVec::Add(m_types, std::move(inst));
	return *m_types.back();
}

DataType *ParentScope::FindDataType(const std::string& id)
{
	return FindInVec(m_types, id);
}

ClassTemplate *ParentScope::FindClassTemplate(const std::string& id)
{
	return FindInVec(m_temps, id);
}
	
} // end of namespace
