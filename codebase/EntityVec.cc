/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "EntityVec.hh"

namespace codebase {

EntityVec::EntityVec( const std::string& name, const std::string& usr, const Entity* parent) :
	m_name{name},
	m_id{usr},
	m_parent{parent}
{
	assert(m_parent || usr == NullID());
}

const std::string& EntityVec::Name() const
{
	return m_name;
}

const std::string& EntityVec::ID() const
{
	return m_id;
}

const Entity* EntityVec::Parent() const
{
	return m_parent;
}

std::size_t EntityVec::ChildCount() const
{
	return m_children.size();
}

const Entity* EntityVec::Child(std::size_t idx) const
{
	return m_children.at(idx).get();
}

Entity* EntityVec::Child(std::size_t idx)
{
	return m_children.at(idx).get();
}

std::size_t EntityVec::IndexOf(const Entity* child) const
{
	auto it = std::find_if(m_children.begin(), m_children.end(), [child](auto& c){return c.get() == child;});
	return it != m_children.end() ? static_cast<std::size_t>(it - m_children.begin()) : npos;
}

void EntityVec::MarkUsed()
{
	m_used = true;
	for (auto& child : m_children)
		child->MarkUsed();
}

bool EntityVec::IsUsed() const
{
	return m_used ||
		std::find_if(m_children.begin(), m_children.end(), [](auto& child){return child->IsUsed();}) != m_children.end();
}

void EntityVec::MarkSelfUsedOnly()
{
	m_used = true;
}
	
} // end of namespace
