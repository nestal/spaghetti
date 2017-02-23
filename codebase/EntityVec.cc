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

EntityVec::EntityVec( const std::string& name, const std::string& usr, const std::string& parent) :
	m_name{name},
	m_parent{parent},
	m_id{usr}
{
}

const std::string& EntityVec::Name() const
{
	return m_name;
}

const std::string& EntityVec::ID() const
{
	return m_id;
}

const std::string& EntityVec::Parent() const
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

Entity* EntityVec::Add(EntityVec::EntityPtr&& child)
{
	m_children.push_back(std::move(child));
	return m_children.back().get();
}

Entity* EntityVec::Find(const std::string& id)
{
	auto it = std::find_if(m_children.begin(), m_children.end(), [id](auto& c){return c->ID() == id;});
	return it != m_children.end() ? it->get() : nullptr;
}

} // end of namespace
