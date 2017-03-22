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

#include <unordered_map>

namespace codebase {

class EntityVec : public Entity
{
public:
	EntityVec() = default;
	EntityVec(const std::string& name, const std::string& usr, const EntityVec *parent);
	
	EntityVec(EntityVec&& other);
	EntityVec(const EntityVec&) = delete;
	EntityVec& operator=(EntityVec&& other);
	EntityVec& operator=(const EntityVec&) = delete;

	const std::string& Name() const override;
	const std::string& ID() const override;
	const EntityVec* Parent() const override;
	
	std::size_t ChildCount() const override ;

	void MarkUsed() override;
	bool IsUsed() const override;
	void Reparent(const EntityVec *parent) override;

	const Entity* FindByID(const std::string& id) const;
	Entity* FindByID(const std::string& id);
	
protected:
	void MarkSelfUsedOnly();

	template <typename Type, typename MultiCond, typename... Ts>
	Type& AddUnique(MultiCond& cond, const std::string& id, Ts... ts)
	{
		auto it = m_index.find(id);
		if (it != m_index.end())
		{
			// can be find inside cond
			auto entity = Child(it->second);
			assert(entity);
			return dynamic_cast<Type&>(*entity);
		}
		else
		{
			m_index.emplace(id, m_index.size());
			return Add(cond, Type{ts...});
		}
	}
	
private:
	std::string m_name;
	std::string m_id{NullID()};
	const EntityVec *m_parent{};
	bool m_used{false};
	
	std::unordered_map<std::string, std::size_t> m_index;
};

} // end of namespace codebase
