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

class EntityVec : public Entity
{
public:
	using EntityPtr = std::unique_ptr<Entity>;

public:
	EntityVec( const std::string& name, const std::string& usr, const std::string& parent);
	
	EntityVec(EntityVec&&) = default;
	EntityVec(const EntityVec&) = delete;
	EntityVec& operator=(EntityVec&&) = default;
	EntityVec& operator=(const EntityVec&) = delete;

	const std::string& Name() const override;
	const std::string& ID() const override;
	const std::string& Parent() const override;
	
	std::size_t ChildCount() const override ;
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;
	
	Entity* Add(EntityPtr&& child);
	Entity* Find(const std::string& id);
	
private:
	std::string m_name;
	std::string m_parent;
	std::string m_id;

	std::vector<EntityPtr> m_children;
};

} // end of namespace
