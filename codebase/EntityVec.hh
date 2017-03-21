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

#include <tuple>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <cassert>
#include <vector>
#include <memory>

namespace codebase {

template <typename... EntityTypes>
struct EntitysVec;

template <typename LastType>
struct EntitysVec<LastType>
{
	virtual const Entity* At(std::size_t idx) const
	{
		return &m_types.at(idx);
	}
	virtual Entity* At(std::size_t idx)
	{
		return &m_types.at(idx);
	}
	void Add(LastType&& entity)
	{
		m_types.push_back(std::move(entity));
	}
	
	std::vector<LastType> m_types;
};

template <typename FirstType, typename... OtherTypes>
struct EntitysVec<FirstType, OtherTypes...> : public EntitysVec<OtherTypes...>
{
	const Entity* At(std::size_t idx) const override
	{
		if (idx < m_types.size())
			return &m_types.at(idx);
		else
			return EntitysVec<OtherTypes...>::At(idx-m_types.size());
	}
	Entity* At(std::size_t idx) override
	{
		if (idx < m_types.size())
			return &m_types.at(idx);
		else
			return EntitysVec<OtherTypes...>::At(idx-m_types.size());
	}
	void Add(FirstType&& entity)
	{
		m_types.push_back(std::move(entity));
	}
	
	std::vector<FirstType>      m_types;
};

template <typename T, typename... OtherTypes>
std::vector<T>& GetHelper(EntitysVec<T, OtherTypes...>& vec)
{
	return vec.m_types;
}

template <typename T, typename... Types>
std::vector<T>& Get(EntitysVec<Types...>& vec)
{
	return GetHelper<T>(vec);
}

template <typename T, typename... Types>
void Add(EntitysVec<Types...>& vec, T&& entity)
{
	GetHelper<T>(vec).push_back(std::forward<T>(entity));
}

class EntityVec : public Entity
{
private:
	struct ByID {};
	struct BySelf {};
	struct ByIndex {};
	struct ByType {};
	
	struct EntityEntry
	{
		UniqueEntityPtr ptr;
		
		EntityEntry(UniqueEntityPtr&& e) : ptr{std::move(e)} {}
		
		EntityType Type() const {return ptr->Type();}
		const std::string& ID() const {return ptr->ID();}
		const Entity* Self() const {return ptr.get();}
	};
	
	using EntityIndex = boost::multi_index_container<
		EntityEntry,
		boost::multi_index::indexed_by<
			
			// hash by ID
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<ByID>,
				boost::multi_index::const_mem_fun<
					EntityEntry,
					const std::string&,
					&EntityEntry::ID
				>
			>,
			
			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<BySelf>,
				boost::multi_index::const_mem_fun<
					EntityEntry,
					const Entity*,
					&EntityEntry::Self
				>
			>,
			
			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<ByType>,
				boost::multi_index::const_mem_fun<
					EntityEntry,
					EntityType,
					&EntityEntry::Type
				>
			>,
			
			boost::multi_index::random_access<boost::multi_index::tag<ByIndex>>
		>
	>;
	
public:
	using TypeIterator = typename EntityIndex::index<ByType>::type::iterator;

protected:
	template <typename DestEntity>
	struct DynamicCastEntity : std::unary_function<const Entity&, const EntityEntry&>
	{
		const DestEntity& operator()(const EntityEntry& e) const
		{
			return dynamic_cast<const DestEntity&>(*e.ptr);
		}
	};
	
	template <typename DestEntity>
	using EntityTypeIterator = boost::iterators::transform_iterator<
		DynamicCastEntity<DestEntity>,
		TypeIterator,
		const DestEntity&
	>;
	
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
	const Entity* Child(std::size_t idx) const override;
	std::size_t IndexOf(const Entity* child) const override;
	void UpdateChild(const Entity* child, const std::function<void(Entity*)>& mod) override;

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
	
	template <typename Mod>
	void Update(const Entity *child, Mod mod)
	{
		auto&& idx = m_db.get<BySelf>();
		auto it = idx.find(child);
		if (it != idx.end())
			idx.modify(it, [&mod](EntityEntry& ee) {mod(ee.ptr.get()); });
	}
	
	void MarkUsed() override;
	bool IsUsed() const override;
	void Reparent(const EntityVec *parent) override;

	const Entity* FindByID(const std::string& id) const;
	
	bool Modify(const std::string& id, std::function<void(Entity*)> func);
	
	boost::iterator_range<TypeIterator> FindByType(EntityType type) const;
	
protected:
	void MarkSelfUsedOnly();

private:
	std::string m_name;
	std::string m_id{NullID()};
	const EntityVec *m_parent{};
	bool m_used{false};
	
	EntityIndex m_db;
};

template <typename EntityContainer, typename... Args>
auto EntityVec::AddUnique(EntityContainer&& cont, const std::string& id, Args... arg)
{
	using Type = std::remove_pointer_t<typename std::remove_reference_t<EntityContainer>::value_type>;

	auto entity = dynamic_cast<Type*>(const_cast<Entity*>(FindByID(id)));
	if (!entity)
	{
		entity = Add<Type>(std::forward<Args>(arg)...);
		cont.push_back(entity);
	}
	return entity;
}

} // end of namespace codebase
