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

#include <vector>

namespace codebase {

template <typename EntityType>
class EntityVec : public Entity
{
public:
	EntityVec(
		const Entity *parent,
		const std::string& name
	) : m_parent{parent}, m_name{name} {}
	
	const std::string& Name() const override {return m_name;}
	const Entity* Parent() const override {return m_parent;}
	
	std::size_t ChildCount() const override {return m_children.size();}
	const EntityType* Child(std::size_t idx) const override {return &m_children.at(idx);}
	std::size_t IndexOf(const Entity* child) const override {return &dynamic_cast<const EntityType&>(*child) - &m_children[0];}
	
private:
	const Entity *m_parent;
	std::string m_name;
	
	std::vector<EntityType> m_children;
};

} // end of namespace
