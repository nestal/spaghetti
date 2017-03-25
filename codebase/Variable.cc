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
	m_type{field.Type()}
{
}


EntityType Variable::Type() const
{
	return EntityType::variable;
}

std::ostream& operator<<(std::ostream& os, const Variable& c)
{
	return os << c.m_type << " " << c.Name();
}

libclx::SourceLocation Variable::Location() const
{
	return m_location;
}

std::string Variable::UML() const
{
	return Name() + " : " + DisplayType();
}

std::string Variable::TypeID() const
{
	return m_type.Declaration().USR();
}

void Variable::CrossReference(EntityMap *map)
{
	if (IsUsed())
	{
		if (auto type = dynamic_cast<const DataType *>(map->Find(m_type.Declaration().USR())))
			const_cast<DataType*>(type)->MarkUsed();
	}
}

std::string Variable::DisplayType() const
{
	return m_type.Spelling();
}
	
} // end of namespace
