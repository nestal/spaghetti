/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#pragma once

#include "Entity.hh"

#include <cassert>
#include <vector>
#include <memory>

namespace codebase {

template <typename EntityType>
class EntityVec : public Entity
{
public:
	using Vec = std::vector<EntityType>;
	using iterator       = typename Vec::iterator;
	using const_iterator = typename Vec::const_iterator;
	
public:
	EntityVec( const std::string& name) : m_name{name} {}
	EntityVec(EntityVec&&) = default;
	EntityVec(const EntityVec&) = delete;
	EntityVec& operator=(EntityVec&&) = default;
	EntityVec& operator=(const EntityVec&) = delete;

	const std::string& Name() const override {return m_name;}
	std::string Type() const override {return {};}
	const Entity* Parent() const override
	{
		assert(m_parent && m_parent->HasChild(this));
		return m_parent;
	}
	void OnReparent(const Entity *parent) override
	{
		assert(m_parent == parent || m_parent == nullptr);
		m_parent = parent;
	}
	
	std::size_t ChildCount() const override {return m_children.size();}
	const EntityType* Child(std::size_t idx) const override
	{
		return &m_children.at(idx);
	}
	EntityType* Child(std::size_t idx) override
	{
		return &m_children.at(idx);
	}
	std::size_t IndexOf(const Entity* child) const override {return &dynamic_cast<const EntityType&>(*child) - &m_children[0];}
	
	iterator begin() {return m_children.begin();}
	iterator end() {return m_children.end();}
	const_iterator begin() const {return m_children.begin();}
	const_iterator end() const {return m_children.end();}
	
	iterator Add(EntityType&& ent)
	{
		m_children.push_back(std::move(ent));
		return --m_children.end();
	}
	
private:
	const Entity *m_parent{};
	std::string m_name;
	
	std::vector<EntityType> m_children;
};

} // end of namespace
