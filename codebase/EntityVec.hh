/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
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
	EntityVec( const std::string& name, const std::string& parent) :
		m_name{name},
		m_parent{parent},
		m_id{parent + "EntityVec-" + name}
	{
	}
	
	EntityVec(EntityVec&&) = default;
	EntityVec(const EntityVec&) = delete;
	EntityVec& operator=(EntityVec&&) = default;
	EntityVec& operator=(const EntityVec&) = delete;

	const std::string& Name() const override {return m_name;}
	std::string Type() const override {return {};}
	const std::string& ID() const override
	{
		return m_id;
	}
	const std::string& Parent() const override
	{
		return m_parent;
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
	std::size_t IndexOf(const Entity* child) const override
	{
		return &dynamic_cast<const EntityType&>(*child) - &m_children[0];
	}
	
	using iterator       = EntityIterator<EntityVec, EntityType>;
	using const_iterator = EntityIterator<const EntityVec, const EntityType>;
	
	iterator begin() {return iterator{0,                 this};}
	iterator end()   {return iterator{m_children.size(), this};}
	const_iterator begin() const {return const_iterator{0,                 this};}
	const_iterator end()   const {return const_iterator{m_children.size(), this};}
	
	template <typename... Args>
	iterator Add(Args... arg)
	{
		m_children.emplace_back(std::forward<Args>(arg)...);
		return --end();
	}
	
private:
	std::string m_name;
	std::string m_parent;
	std::string m_id;
	
	std::vector<EntityType> m_children;
};

} // end of namespace
