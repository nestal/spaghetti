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

#include "Function.hh"
#include "Variable.hh"
#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"

#include <ostream>
#include <iostream>

namespace codebase {

ParentScope::ParentScope(const libclx::Cursor& cursor, const Entity* parent) :
	EntityVec{cursor.Spelling(), cursor.USR(), parent}
{
}

ParentScope::ParentScope(const std::string& name, const std::string& usr, const Entity *parent) :
	EntityVec{name, usr, parent}
{
}

void ParentScope::Visit(const libclx::Cursor& self)
{
	OnVisit(self);
	self.Visit([this](auto child, auto parent){VisitChild(child, parent);});
	AfterVisitingChild(self);
}

void ParentScope::VisitChild(const libclx::Cursor& child, const libclx::Cursor& self)
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
		AddUnique(m_functions, child.USR(), child, this);
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
	return {m_functions.begin(), m_functions.end()};
}

const codebase::Function& ParentScope::Function(std::size_t idx) const
{
	return *m_functions.at(idx);
}

const codebase::Variable& ParentScope::Field(std::size_t idx) const
{
	return *m_fields.at(idx);
}

void ParentScope::OnVisit(const libclx::Cursor&)
{
}

void ParentScope::AfterVisitingChild(const libclx::Cursor&)
{
	
}

void ParentScope::VisitFunction(const libclx::Cursor& func)
{
	auto it = FindByID(m_functions, func.USR());
	
	// the definition of a member function should come after its declaration
	if (it != m_functions.end())
		(*it)->Visit(func);
}

std::vector<DataType *>& ParentScope::Types()
{
	return m_types;
}

void ParentScope::Add(std::unique_ptr<DataType>&& type)
{
	m_types.push_back(type.get());
	
}
	
} // end of namespace
