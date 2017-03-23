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

#include "EntityVec.hh"

#include "util/MultiContainer.hh"

#include "libclx/Type.hh"
#include "libclx/SourceRange.hh"

namespace libclx {
class Cursor;
}

namespace codebase {

class Variable;

class Function : public EntityVec
{
public:
	Function(const libclx::Cursor& first_seen, const EntityVec *parent);
	Function(Function&&);
	Function& operator=(Function&&);
	
	void Visit(const libclx::Cursor& self);
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;
	std::size_t ChildCount() const override;
	
	EntityType Type() const override;
	libclx::SourceLocation Location() const override;
	libclx::Type ReturnType() const;
	
	void CrossReference(EntityMap *map) override;
	
	std::string UML() const override;
	
private:
	libclx::SourceLocation  m_definition;
	libclx::Type            m_return_type;
	
	util::MultiContainer<Entity, Variable>  m_args;
};
	
} // end of namespace
