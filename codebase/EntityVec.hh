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
	EntityVec() = default;
	EntityVec(const std::string& name, const std::string& usr, const Entity *parent);
	
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
		m_children.push_back(std::move(child));
		return ptr;
	}
	
	template <typename EntityContainer, typename... Args>
	auto AddUnique(EntityContainer&& cont, const std::string& id, Args... arg);
	
	void MarkUsed() override;
	bool IsUsed() const override;
	void Reparent(const Entity *parent) override;

protected:
	void MarkSelfUsedOnly();

private:
	std::string m_name;
	std::string m_id{NullID()};
	const Entity *m_parent{};
	bool m_used{false};

	std::vector<EntityPtr> m_children;
};

template <typename EntityContainer>
auto FindByID(EntityContainer&& cont, const std::string& id)
{
	return std::find_if(cont.begin(), cont.end(), [id](auto& e){return e->ID() == id;});
}

template <typename EntityContainer, typename... Args>
auto EntityVec::AddUnique(EntityContainer&& cont, const std::string& id, Args... arg)
{
	using Type = std::remove_pointer_t<typename std::remove_reference_t<EntityContainer>::value_type>;

	auto it = FindByID(cont, id);
	if (it == cont.end())
	{
		cont.push_back(Add<Type>(std::forward<Args>(arg)...));
		it = --cont.end();
	}
	return *it;
}

} // end of namespace codebase
