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

EntityVec::EntityVec( const std::string& name, const std::string& usr, const EntityVec* parent) :
	m_name{name},
	m_id{usr},
	m_parent{parent}
{
	assert(m_parent || usr == NullID());
}

EntityVec::EntityVec(EntityVec&& other) :
	m_name{std::move(other.m_name)},
	m_id{std::move(other.m_id)},
	m_parent{other.m_parent},
	m_used{other.m_used}
{
}

EntityVec& EntityVec::operator=(EntityVec&& other)
{
	m_name      = std::move(other.m_name);
	m_id        = std::move(other.m_id);
	m_parent    = other.m_parent;
	m_used      = other.m_used;
	
	return *this;
}

const std::string& EntityVec::Name() const
{
	return m_name;
}

const std::string& EntityVec::ID() const
{
	return m_id;
}

const EntityVec* EntityVec::Parent() const
{
	return m_parent;
}

void EntityVec::MarkUsed()
{
	m_used = true;
	for (auto&& child : *this)
		child.MarkUsed();
}

bool EntityVec::IsUsed() const
{
	return m_used ||
		std::find_if(begin(), end(), [](auto& child){return child.IsUsed();}) != end();
}

void EntityVec::MarkSelfUsedOnly()
{
	m_used = true;
}

void EntityVec::Reparent(const EntityVec *parent)
{
	m_parent = parent;
}

const Entity *EntityVec::FindByID(const std::string& id) const
{
	auto it = m_index.find(id);
	return it != m_index.end() && it->second < ChildCount() ? Child(it->second) : nullptr;
		
}

Entity *EntityVec::FindByID(const std::string& id)
{
	auto it = m_index.find(id);
	return it != m_index.end() && it->second < ChildCount() ? Child(it->second) : nullptr;
	
}
	
} // end of namespace
