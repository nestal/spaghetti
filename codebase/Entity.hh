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

#include <boost/iterator/iterator_facade.hpp>

#include <functional>
#include <string>
#include <type_traits>

#include <libclx/Index.hh>

namespace codebase {

template <typename Parent, typename Child>
class EntityIterator;

class EntityVec;

class EntityMap;
enum class EntityType;

/**
 * \brief Represent an entity in the code base.
 *
 * An Entity is an addressable thing in the code base. It can be a class, a data member, a
 * member function, a namespace and anything. Entities are organized in a tree structure.
 * Each Entity has a parent (except for the root) and optionally a list of child entities.
 *
 * Each Entity has an ID to unique identify them in the code base. In libclang-c this ID
 * is called an USR. In other words, the definition of an Entity is a libclang-c object
 * with a unique USR. In libclang-c terms, the parent-child relationship corresponds to
 * the semantic-parent relationship, not the lexical one.
 */
class Entity
{
public:
	virtual ~Entity() = default;
	
	static const std::string& NullID();
	
	/**
	 * \brief Name of the entity.
	 *
	 * If the entity is a class, this function returns its class name. Similarly it returns
	 * the function name if the entity is a function. The namespace prefix will not be
	 * included in any case.
	 *
	 * \return
	 */
	virtual const std::string& Name() const = 0;
	virtual const std::string& ID() const = 0;
	
	/**
	 * \brief Returns the parent of this entity.
	 * \return Parent entity.
	 *
	 * The parent entity is the one who contains this entity. For example, the parent entity
	 * of a class is the namespace that contains it. The parent entity of a member function
	 * is the class. For non-member functions, the parent entity will be a namespace.
	 */
	virtual const EntityVec* Parent() const = 0;
	virtual EntityType Type() const = 0;
	virtual std::string DisplayType() const;
	
	virtual std::size_t ChildCount() const = 0;
	virtual const Entity* Child(std::size_t idx) const = 0;
	virtual Entity* Child(std::size_t idx) = 0;
	virtual std::size_t IndexOf(const Entity* child) const = 0;
	
	virtual bool IsUsed() const = 0;
	virtual void CrossReference(EntityMap *map) = 0;
	
	virtual libclx::SourceLocation Location() const;
	virtual std::string UML() const;
	
	static const std::size_t npos;
	
	using iterator = EntityIterator<Entity, Entity>;
	using const_iterator = EntityIterator<const Entity, const Entity>;
	
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	
	virtual void Reparent(const EntityVec *parent) = 0;
	
public:
	bool HasChild(const Entity *child) const {return IndexOf(child) < ChildCount();}
};

using UniqueEntityPtr = std::unique_ptr<Entity>;

template <typename ParentEntity, typename ChildEntity>
class EntityIterator : public boost::iterator_facade<
	EntityIterator<ParentEntity, ChildEntity>,
	ChildEntity,
	boost::random_access_traversal_tag
>
{
public:
	EntityIterator() = default;
	EntityIterator(std::size_t idx, ParentEntity *parent) :
		m_idx{idx},
		m_parent{parent}
	{}

	// iterators can be converted if the parent and child entity types
	// can be converted.
	// for example, non-const iterator can be converted to const_iterator.
	template <
		typename OtherParentEntity, typename OtherChildEntity,
		typename std::enable_if<
			std::is_convertible<OtherParentEntity*, ParentEntity*>::value &&
			std::is_convertible<OtherChildEntity*,  ChildEntity* >::value
		>::type* = nullptr
	>
	EntityIterator(const EntityIterator<OtherParentEntity, OtherChildEntity>& other) :
		m_idx{other.m_idx}, m_parent{other.m_parent}
	{
	}
	
private:
	friend class boost::iterator_core_access;
	template <typename P, typename C>
	friend class EntityIterator;
	
	void increment() {m_idx++;}
	void decrement() {m_idx--;}
	void advance(std::ptrdiff_t n) {m_idx += n;}
	std::ptrdiff_t distance_to(const EntityIterator& other) const {return other.m_idx - m_idx;}
	bool equal(const EntityIterator& other) const
	{
		return m_idx == other.m_idx && m_parent == other.m_parent;
	}
	
	ChildEntity& dereference() const
	{
		return *m_parent->Child(m_idx);
	}

private:
	std::size_t  m_idx{};
	ParentEntity *m_parent{};
};

class LeafEntity : public Entity
{
public:
	LeafEntity(const std::string& name, const std::string& usr, const EntityVec *parent);
	
	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;

	const std::string& Name() const override;
	const EntityVec* Parent() const override;
	const std::string& ID() const override;
	
	bool IsUsed() const override;
	void CrossReference(EntityMap *map) override;

	void Reparent(const EntityVec *entity) override;

private:
	std::string m_name;
	std::string m_usr;
	const EntityVec *m_parent;
};

} // end of namespace
