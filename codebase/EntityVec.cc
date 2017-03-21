/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "EntityVec.hh"

#include "EntityType.hh"

#include <functional>

namespace codebase {

namespace mi = boost::multi_index;

EntityVec::EntityVec( const std::string& name, const std::string& usr, const EntityVec* parent) :
	m_name{name},
	m_id{usr},
	m_parent{parent}
{
	assert(m_parent || usr == NullID());
}

EntityVec::EntityVec(EntityVec&& other) :
	m_name{std::move(other.m_name)},
	m_id{std::move(other.m_id)},
	m_parent{other.m_parent},
	m_used{other.m_used}
{
	for (auto&& c : m_db)
		c.ptr->Reparent(this);
}

EntityVec& EntityVec::operator=(EntityVec&& other)
{
	m_name      = std::move(other.m_name);
	m_id        = std::move(other.m_id);
	m_parent    = other.m_parent;
	m_used      = other.m_used;
	
	for (auto&& c : m_db)
		c.ptr->Reparent(this);
	
	return *this;
}

const std::string& EntityVec::Name() const
{
	return m_name;
}

const std::string& EntityVec::ID() const
{
	return m_id;
}

const EntityVec* EntityVec::Parent() const
{
	return m_parent;
}

std::size_t EntityVec::ChildCount() const
{
	return m_db.size();
}

const Entity* EntityVec::Child(std::size_t idx) const
{
	return m_db.get<ByIndex>().at(idx).Self();
}

std::size_t EntityVec::IndexOf(const Entity* child) const
{
	auto&& idx = m_db.get<BySelf>();
	auto it = idx.find(child);
	return it != idx.end()
		? mi::project<ByIndex>(m_db, it) - m_db.get<ByIndex>().begin()
		: Entity::npos;
}

void EntityVec::MarkUsed()
{
	m_used = true;
	for (auto& child : m_db)
		child.ptr->MarkUsed();
}

bool EntityVec::IsUsed() const
{
	return m_used ||
		std::find_if(m_db.begin(), m_db.end(), [](auto& child){return child.ptr->IsUsed();}) != m_db.end();
}

void EntityVec::MarkSelfUsedOnly()
{
	m_used = true;
}

void EntityVec::Reparent(const EntityVec *parent)
{
	m_parent = parent;
}

void EntityVec::AddChild(UniqueEntityPtr&& child)
{
	m_db.insert(std::move(child));
}

const Entity *EntityVec::FindByID(const std::string& id) const
{
	auto&& idx = m_db.get<ByID>();
	auto it = idx.find(id);
	return it != idx.end() ? it->ptr.get() : nullptr;
		
}

boost::iterator_range<EntityVec::TypeIterator> EntityVec::FindByType(EntityType type) const
{
	auto range = m_db.get<ByType>().equal_range(type);
	return {range.first, range.second};
}

bool EntityVec::Modify(const std::string& id, std::function<void(Entity*)> func)
{
	auto&& idx = m_db.get<ByID>();
	auto it = idx.find(id);
	if (it != idx.end())
	{
		idx.modify(it, [&func](auto&& ee) { func(ee.ptr.get()); });
		return true;
	}
	else
		return false;
}

} // end of namespace
