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

#include "libclx/Index.hh"

#include <boost/optional.hpp>

#include <memory>
#include <vector>

namespace codebase {

class Entity;

class EditAction
{
public:
	enum class Action {addEntity, setDefinition};
	
	EditAction() = default;
	~EditAction();
	EditAction(EditAction&&) = default;
	EditAction(const EditAction&) = default;
	
	template <typename EntityType>
	void AddEntity(EntityType&& entity)
	{
		m_ent.emplace_back(Action::addEntity);
		m_ent.back().entity = std::make_unique<EntityType>(std::forward<EntityType>(entity));
	}
	
	void SetDefinition(const libclx::SourceLocation& loc);
	
	template <typename Func>
	void ForEach(Func func)
	{
		for (auto&& e : m_ent)
			func(e.type, e.entity, e.location);
	}

private:
	struct Entry
	{
		Entry(Action t) : type{t} {}
		
		Action type;
		std::unique_ptr<Entity> entity;
		libclx::SourceLocation  location;
	};
	std::vector<Entry> m_ent;
};
	
} // end of namespace
