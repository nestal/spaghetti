/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "Variable.hh"

namespace codebase {

Variable::Variable(libclx::Cursor field, const Entity *parent) :
	m_name{field.Spelling()},
	m_usr{field.USR()},
	m_parent{parent},
	m_type{field.Type()}
{
}

const std::string& Variable::Name() const
{
	return m_name;
}

const Entity* Variable::Parent() const
{
	return m_parent;
}

const std::string& Variable::USR() const
{
	return m_usr;
}

std::size_t Variable::ChildCount() const
{
	return 0;
}

const Entity *Variable::Child(std::size_t) const
{
	return nullptr;
}

std::size_t Variable::IndexOf(const Entity*) const
{
	return 0;
}

std::string Variable::Type() const
{
	return m_type.Spelling();
}

std::ostream& operator<<(std::ostream& os, const Variable& c)
{
	return os << c.m_name << ": " << c.m_type;
}

} // end of namespace
