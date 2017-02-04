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

#include <string>

namespace codebase {

class Entity
{
public:
	virtual ~Entity() = default;
	
	virtual const std::string& Name() const = 0;
	virtual std::string Type() const = 0;
	virtual const Entity* Parent() const = 0;
	virtual void Reparent(const Entity *parent) = 0;
	
	virtual std::size_t ChildCount() const = 0;
	virtual const Entity* Child(std::size_t idx) const = 0;
	virtual std::size_t IndexOf(const Entity* child) const = 0;
	
public:
	bool HasChild(const Entity *child) const {return IndexOf(child) < ChildCount();}
};
	
} // end of namespace
