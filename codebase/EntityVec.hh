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
#include <memory>

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
	
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;
	std::size_t ChildCount() const override;
		
	bool IsUsed() const override;
	void Reparent(const EntityVec *parent) override;

	const Entity* FindByID(const std::string& id) const;
	Entity* FindByID(const std::string& id);
	
protected:
	template <typename EntityContainer, typename... Ts>
	auto AddUnique(EntityContainer&& cond, const std::string& id, Ts&&... ts) ->
	typename std::remove_reference_t<EntityContainer>::value_type
	{
		using Type = std::remove_pointer_t<typename std::remove_reference_t<EntityContainer>::value_type>;
		
		auto it = m_index.find(id);
		if (it != m_index.end())
		{
			// can be find inside cond
			assert(it->second.typed < cond.size());
			return cond.at(it->second.typed);
		}
		else
		{
			Add(cond, std::make_unique<Type>(std::forward<Ts>(ts)...));
			return cond.back();
		}
	}
	
	template <typename EntityContainer, typename ET>
	void Add(EntityContainer&& cond, std::unique_ptr<ET>&& entity)
	{
		assert(entity->Parent() == this);
		
		auto self  = m_children.size();
		auto typed = cond.size();
		
		cond.push_back(entity.get());
		m_children.push_back(std::move(entity));
		
		// if push_back() throw, do not add to index
		m_index.emplace(cond.back()->ID(), Indexes{self, typed});
	}
	
	template <typename EntityContainer, typename... Ts>
	auto FindInVec(EntityContainer&& cond, const std::string& id) ->
	typename std::remove_reference_t<EntityContainer>::value_type
	{
		auto it = m_index.find(id);
		return it != m_index.end() && it->second.typed < cond.size() ? cond.at(it->second.typed) : nullptr;
	}
	
	void SetUsed(bool used = true);
	
private:
	using EntityPtr = std::unique_ptr<Entity>;
	
private:
	std::string m_name;
	std::string m_id{NullID()};
	const EntityVec *m_parent{};
	bool m_used{false};
	
	struct Indexes
	{
		std::size_t self;   //!< index of entity in our m_children vector
		std::size_t typed;  //!< index of entity in the vector passed to AddUnique() when it's created
	};
	
	std::unordered_map<std::string, Indexes> m_index;
	std::vector<EntityPtr> m_children;
};

} // end of namespace codebase
