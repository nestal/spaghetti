/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "Entity.hh"
#include "libclx/SourceRange.hh"

#include <cassert>

namespace codebase {

const std::size_t Entity::npos = std::string::npos;

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

const std::string& Entity::NullID()
{
	static const std::string id;
	return id;
}

libclx::SourceLocation Entity::Location() const
{
	return libclx::SourceLocation();
}

bool Entity::IsUsed() const
{
	return std::find_if(begin(), end(), [](auto& child){return child.IsUsed();}) != end();
}

std::string Entity::UML() const
{
	return Name();
}

LeafEntity::LeafEntity(const std::string& name, const std::string& usr, const Entity *parent) :
	m_name{name},
	m_usr{usr},
	m_parent{parent}
{
	assert(m_parent);
}

std::size_t LeafEntity::ChildCount() const
{
	return 0;
}

const Entity *LeafEntity::Child(std::size_t) const
{
	return nullptr;
}

Entity *LeafEntity::Child(std::size_t)
{
	return nullptr;
}

std::size_t LeafEntity::IndexOf(const Entity*) const
{
	return npos;
}

const std::string& LeafEntity::Name() const
{
	return m_name;
}

const Entity* LeafEntity::Parent() const
{
	return m_parent;
}

const std::string& LeafEntity::ID() const
{
	return m_usr;
}

bool LeafEntity::IsUsed() const
{
	return Location().IsFromMainFile();
}

void LeafEntity::RemoveUnused()
{
}

} // end of namespace
