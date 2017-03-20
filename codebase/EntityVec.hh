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
	EntityVec();
	EntityVec(const std::string& name, const std::string& usr, const Entity *parent);
	~EntityVec();
	
	EntityVec(EntityVec&& other);
	EntityVec(const EntityVec&) = delete;
	EntityVec& operator=(EntityVec&& other);
	EntityVec& operator=(const EntityVec&) = delete;

	const std::string& Name() const override;
	const std::string& ID() const override;
	const Entity* Parent() const override;
	
	std::size_t ChildCount() const override ;
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;
	
	template <typename Type, typename... Args>
	Type* Add(Args... arg)
	{
		auto child = std::make_unique<Type>(std::forward<Args>(arg)...);
		auto ptr   = child.get();
		AddChild(std::move(child));
		return ptr;
	}
	
	template <typename EntityContainer, typename... Args>
	auto AddUnique(EntityContainer&& cont, const std::string& id, Args... arg);
	
	void AddChild(UniqueEntityPtr&& child);
	
	void MarkUsed() override;
	bool IsUsed() const override;
	void Reparent(const Entity *parent) override;

	const Entity* Find(const std::string& id) const;
	
	bool Modify(const std::string& id, std::function<void(Entity*)> func);
	
protected:
	void MarkSelfUsedOnly();

private:
	std::string m_name;
	std::string m_id{NullID()};
	const Entity *m_parent{};
	bool m_used{false};

	struct Container;
	std::unique_ptr<Container>   m_db;
};

template <typename EntityContainer, typename... Args>
auto EntityVec::AddUnique(EntityContainer&& cont, const std::string& id, Args... arg)
{
	using Type = std::remove_pointer_t<typename std::remove_reference_t<EntityContainer>::value_type>;

	auto entity = dynamic_cast<Type*>(const_cast<Entity*>(Find(id)));
	if (!entity)
	{
		entity = Add<Type>(std::forward<Args>(arg)...);
		cont.push_back(entity);
	}
	return entity;
}

} // end of namespace codebase
