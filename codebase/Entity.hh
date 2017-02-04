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

#include <boost/iterator/iterator_facade.hpp>
#include <string>
#include <type_traits>

namespace codebase {

template <typename Parent, typename Child>
class EntityIterator;

class Entity;

class EntityMap
{
public:
	virtual ~EntityMap() = default;
	
	virtual const Entity* Find(const std::string& id) const = 0;
};

class Entity
{
public:
	virtual ~Entity() = default;
	
	static const std::string& NullID();
	
	virtual const std::string& Name() const = 0;
	virtual const std::string& ID() const = 0;
	virtual const std::string& Parent() const = 0;
	virtual std::string Type() const = 0;
	
	virtual std::size_t ChildCount() const = 0;
	virtual const Entity* Child(std::size_t idx) const = 0;
	virtual Entity* Child(std::size_t idx) = 0;
	virtual std::size_t IndexOf(const Entity* child) const = 0;

	using iterator       = EntityIterator<Entity, Entity>;
	using const_iterator = EntityIterator<const Entity, const Entity>;
	
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	
public:
	bool HasChild(const Entity *child) const {return IndexOf(child) < ChildCount();}
};

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
	
	void increment() {m_idx++;}
	void decrement() {m_idx--;}
	void advance(long n) {m_idx += n;}
	long distance_to(const EntityIterator& other) const {return other.m_idx - m_idx;}
	bool equal(const EntityIterator& other) const
	{
		return m_idx == other.m_idx && m_parent == other.m_parent;
	}
	
	ChildEntity& dereference() const
	{
		return *m_parent->Child(m_idx);
	}

public:
	std::size_t  m_idx{};
	ParentEntity *m_parent{};
};

} // end of namespace
