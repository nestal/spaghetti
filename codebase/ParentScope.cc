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
#include "ParentScopeImpl.hh"

#include "libclx/Cursor.hh"
#include "EntityType.hh"

#include <ostream>
#include <iostream>

namespace codebase {

ParentScope::ParentScope(const libclx::Cursor& cursor, const EntityVec* parent) :
	EntityVec{cursor.DisplayName(), cursor.USR(), parent},
	m_{std::make_unique<Impl>()}
{
}

ParentScope::ParentScope(const std::string& name, const std::string& usr, const EntityVec *parent) :
	EntityVec{name, usr, parent},
	m_{std::make_unique<Impl>()}
{
}

ParentScope::ParentScope(ParentScope&& rhs) :
	EntityVec{std::move(rhs)},
	m_{std::move(rhs.m_)}
{
	for (auto&& c : *this)
		c.Reparent(this);
}

ParentScope& ParentScope::operator=(ParentScope&& rhs)
{
	EntityVec::operator=(std::move(rhs));
	m_ = std::move(rhs.m_);
	
	for (auto&& c : *this)
		c.Reparent(this);
	
	return *this;
}

ParentScope::~ParentScope() = default;

void ParentScope::Visit(const libclx::Cursor& self)
{
	OnVisit(self);
	self.Visit([this](auto child, auto parent){this->VisitChild(child, parent);});
	AfterVisitingChild(self);
}

void ParentScope::VisitChild(const libclx::Cursor& child, const libclx::Cursor&)
{
	switch (child.Kind())
	{
	case CXCursor_FieldDecl:
		AddUnique<Variable>(m_->cond, child.USR(), child, this);
		break;
		
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
	std::cout << "class declaration: " << child.USR() << std::endl;
		AddUnique<DataType>(m_->cond, child.USR(), child, this).Visit(child);
		break;
	
	case CXCursor_ClassTemplate:
		AddUnique<ClassTemplate>(m_->cond, child.USR(), child, this).Visit(child);
		break;
	
	case CXCursor_CXXMethod:
		AddUnique<codebase::Function>(m_->cond, child.USR(), child, this);
		break;
	
	default:
		break;
	}
}

boost::iterator_range<ParentScope::field_iterator> ParentScope::Fields() const
{
	auto& vec = util::Get<Variable>(m_->cond);
	return {vec.begin(), vec.end()};
}

boost::iterator_range<ParentScope::function_iterator> ParentScope::Functions() const
{
	auto& vec = util::Get<codebase::Function>(m_->cond);
	return {vec.begin(), vec.end()};
}

const codebase::Function& ParentScope::Function(std::size_t idx) const
{
	auto& vec = util::Get<codebase::Function>(m_->cond);
	return vec.at(idx);
}

const codebase::Variable& ParentScope::Field(std::size_t idx) const
{
	auto& vec = util::Get<codebase::Variable>(m_->cond);
	return vec.at(idx);
}

void ParentScope::OnVisit(const libclx::Cursor&)
{
}

void ParentScope::AfterVisitingChild(const libclx::Cursor&)
{
	
}

void ParentScope::VisitFunction(const libclx::Cursor& func)
{
	auto func_entity = dynamic_cast<codebase::Function*>(FindByID(func.USR()));
	
	// the definition of a member function should come after its declaration
	if (func_entity)
		func_entity->Visit(func);
}

const Entity* ParentScope::Child(std::size_t idx) const
{
	return m_->cond.At(idx);
}

Entity* ParentScope::Child(std::size_t idx)
{
	return m_->cond.At(idx);
}

std::size_t ParentScope::IndexOf(const Entity* child) const
{
	return m_->cond.IndexOf(child);
}

std::size_t ParentScope::ChildCount() const
{
	return m_->cond.Size();
}

} // end of namespace
