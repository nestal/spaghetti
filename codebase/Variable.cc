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

#include "libclx/Cursor.hh"

#include <ostream>

namespace codebase {

Variable::Variable(libclx::Cursor field, const Entity *parent) :
	LeafEntity{field.Spelling(), field.USR(), parent},
	m_location{field.Location()},
	m_type{field.Type()}
{
}


std::string Variable::Type() const
{
	return m_type.Spelling();
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
	return Name() + " : " + Type();
}

void Variable::CrossReference(EntityMap *map)
{
	if (auto type = dynamic_cast<DataType*>(map->Find(m_type.Declaration().USR())))
		type->MarkUsed();
}

} // end of namespace
