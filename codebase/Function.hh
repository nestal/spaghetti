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
	Function(const Function&) = delete;
	Function(Function&&) = default;
	Function& operator=(const Function&) = delete;
	Function& operator=(Function&&) = default;
	
	void Visit(const libclx::Cursor& self);
	
	EntityType Type() const override;
	libclx::SourceLocation Location() const override;
	libclx::Type ReturnType() const;
	
	void CrossReference(EntityMap *map) override;
	
	std::string UML() const override;
	
private:
	libclx::SourceLocation  m_definition;
	libclx::Type            m_return_type;
	
	std::vector<Variable*>  m_args;
};
	
} // end of namespace
