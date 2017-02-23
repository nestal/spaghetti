/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "MockEntity.hh"

namespace codebase {
namespace ut {

MockEntity::MockEntity(size_t idx, const Entity* parent) :
	m_index{idx},
	m_name{"mock" + std::to_string(idx)},
	m_parent{parent},
	m_id{"mock" + std::to_string(idx) + "ID"}
{
}

const std::string& MockEntity::Name() const
{
	return m_name;
}

std::size_t MockEntity::Index() const
{
	return m_index;
}

const Entity* MockEntity::Parent() const
{
	return m_parent;
}

const std::string& MockEntity::ID() const
{
	return m_id;
}

std::string MockEntity::Type() const
{
	return "Mock";
}

bool MockEntity::IsUsed() const
{
	return true;
}

void MockEntity::RemoveUnused()
{
}
	
}} // end of namespace
