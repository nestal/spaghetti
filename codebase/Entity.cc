/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "Entity.hh"

#include <cassert>

namespace codebase {

void Entity::Reparent(const Entity *parent)
{
	OnReparent(parent);
	for (auto&& child : *this)
		child.Reparent(this);
}

Entity::iterator Entity::begin()
{
	return iterator(0, this);
}

Entity::iterator Entity::end()
{
	return iterator(ChildCount(), this);
}

Entity::const_iterator Entity::begin() const
{
	return const_iterator(0, this);
}

Entity::const_iterator Entity::end() const
{
	return const_iterator(ChildCount(), this);
}

void Entity::iterator::increment()
{
	m_idx++;
}

bool Entity::iterator::equal(const Entity::iterator& other) const
{
	return m_parent == other.m_parent && m_idx == other.m_idx;
}

Entity& Entity::iterator::dereference() const
{
	return *m_parent->Child(m_idx);
}

Entity::const_iterator::const_iterator(const Entity::iterator& it) :
	m_idx{it.m_idx},
	m_parent{it.m_parent}
{
}

void Entity::const_iterator::increment()
{
	m_idx++;
}

bool Entity::const_iterator::equal(const Entity::iterator& other) const
{
	return m_parent == other.m_parent && m_idx == other.m_idx;
}

const Entity& Entity::const_iterator::dereference() const
{
	return *m_parent->Child(m_idx);
}

} // end of namespace
