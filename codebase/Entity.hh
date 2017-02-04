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

namespace codebase {

class Entity
{
public:
	virtual ~Entity() = default;
	
	virtual const std::string& Name() const = 0;
	virtual std::string Type() const = 0;
	virtual const Entity* Parent() const = 0;
	virtual void OnReparent(const Entity *parent) = 0;
	
	virtual std::size_t ChildCount() const = 0;
	virtual const Entity* Child(std::size_t idx) const = 0;
	virtual Entity* Child(std::size_t idx) = 0;
	virtual std::size_t IndexOf(const Entity* child) const = 0;

	class const_iterator;
	class iterator : public boost::iterator_facade<
		iterator,
		Entity,
		boost::single_pass_traversal_tag
	>
	{
	public:
		iterator() = default;
		iterator(std::size_t idx, Entity *parent) :
			m_idx{idx},
			m_parent{parent}
		{}
	
		friend class const_iterator;
		
	private:
		friend class boost::iterator_core_access;
		
		void increment();
		bool equal(const iterator& other) const;
		
		Entity& dereference() const;
	
	private:
		std::size_t  m_idx{};
		Entity *m_parent{};
	};
	
	class const_iterator : public boost::iterator_facade<
		const_iterator,
		const Entity,
		boost::single_pass_traversal_tag
	>
	{
	public:
		const_iterator() = default;
		const_iterator(std::size_t idx, const Entity *parent) :
			m_idx{idx},
			m_parent{parent}
		{}
	
		const_iterator(const iterator& it);
	
	private:
		friend class boost::iterator_core_access;
		
		void increment();
		bool equal(const iterator& other) const;
		
		const Entity& dereference() const;
	
	private:
		std::size_t  m_idx{};
		const Entity *m_parent{};
	};
	
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	
public:
	bool HasChild(const Entity *child) const {return IndexOf(child) < ChildCount();}
	void Reparent(const Entity *parent);
};
	
} // end of namespace
