/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#pragma once

#include <string>

namespace codebase {

class Entity;
class DataType;
class TypeRef;

class EntityMap
{
public:
	virtual ~EntityMap() = default;
	
	virtual const Entity* Find(const std::string& id) const = 0;
	virtual Entity* Find(const std::string& id) = 0;
	virtual const Entity* FindByName(const std::string& name) const = 0;
	virtual const DataType* Find(const TypeRef& ref) const = 0;
	virtual DataType* Find(const TypeRef& ref) = 0;
	
	virtual DataType* Instantiate(const TypeRef& ref, bool used) = 0;
	
	template <typename EntityType>
	const EntityType* TypedFind(const std::string& id) const
	{
		return dynamic_cast<const EntityType*>(Find(id));
	}
	template <typename EntityType>
	EntityType* TypedFind(const std::string& id)
	{
		return dynamic_cast<EntityType*>(Find(id));
	}
};

} // end of namespace
