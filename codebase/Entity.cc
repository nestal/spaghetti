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

	
} // end of namespace