/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "Variable.hh"

#include "DataType.hh"
#include "EntityMap.hh"
#include "libclx/Cursor.hh"
#include "EntityType.hh"

#include <iostream>

namespace codebase {

Variable::Variable(const libclx::Cursor& field, const EntityVec *parent) :
	LeafEntity{field.Spelling(), field.USR(), parent},
	m_location{field.Location()},
	m_type{field}
{
}

Variable::Variable(const Variable& other, const EntityVec *parent) :
	LeafEntity{other.Name(), other.ID(), parent},
	m_location{other.Location()},
	m_type{other.m_type}
{
	
}

EntityType Variable::Type() const
{
	return EntityType::variable;
}

std::ostream& operator<<(std::ostream& os, const Variable& c)
{
	return os << c.DisplayType() << " " << c.Name();
}

libclx::SourceLocation Variable::Location() const
{
	return m_location;
}

std::string Variable::UML() const
{
	return Name() + " : " + DisplayType();
}

void Variable::CrossReference(EntityMap *)
{
}

std::string Variable::DisplayType() const
{
	return m_type.Name();
}

const TypeRef& Variable::TypeRef() const
{
	return m_type;
}

Variable Variable::ReplaceType(const codebase::TypeRef& type, const EntityVec *parent) const
{
	Variable var{*this, parent};
	var.m_type = type;
	return var;
}

} // end of namespace
