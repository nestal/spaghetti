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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <functional>

namespace codebase {

namespace mi = boost::multi_index;

struct EntityVec::Container
{
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
		mi::indexed_by<
			
			// hash by ID
			mi::hashed_unique<
				mi::tag<ByID>,
				mi::const_mem_fun<
					EntityEntry,
					const std::string&,
					&EntityEntry::ID
				>
			>,
			
			mi::hashed_non_unique<
				mi::tag<BySelf>,
				mi::const_mem_fun<
					EntityEntry,
					const Entity*,
					&EntityEntry::Self
				>
			>,
			
			mi::hashed_non_unique<
			    mi::tag<ByType>,
				mi::const_mem_fun<
					EntityEntry,
					EntityType,
					&EntityEntry::Type
				>
			>,
			
			mi::random_access<mi::tag<ByIndex>>
		>
	>;
	
	EntityIndex db;
};

EntityVec::EntityVec() :
	m_db{std::make_unique<Container>()}
{
}

EntityVec::EntityVec( const std::string& name, const std::string& usr, const Entity* parent) :
	m_name{name},
	m_id{usr},
	m_parent{parent},
	m_db{std::make_unique<Container>()}
{
	assert(m_parent || usr == NullID());
}

EntityVec::EntityVec(EntityVec&& other) :
	m_name{std::move(other.m_name)},
	m_id{std::move(other.m_id)},
	m_parent{other.m_parent},
	m_used{other.m_used},
	m_db{std::move(other.m_db)}
{
	for (auto&& c : m_db->db)
		c.ptr->Reparent(this);
}

EntityVec& EntityVec::operator=(EntityVec&& other)
{
	m_name      = std::move(other.m_name);
	m_id        = std::move(other.m_id);
	m_parent    = other.m_parent;
	m_used      = other.m_used;
	m_db        = std::move(other.m_db);
	
	for (auto&& c : m_db->db)
		c.ptr->Reparent(this);
	
	return *this;
}

EntityVec::~EntityVec() = default;

const std::string& EntityVec::Name() const
{
	return m_name;
}

const std::string& EntityVec::ID() const
{
	return m_id;
}

const Entity* EntityVec::Parent() const
{
	return m_parent;
}

std::size_t EntityVec::ChildCount() const
{
	return m_db->db.size();
}

const Entity* EntityVec::Child(std::size_t idx) const
{
	return m_db->db.get<Container::ByIndex>().at(idx).Self();
}

Entity* EntityVec::Child(std::size_t idx)
{
	return const_cast<Entity*>(m_db->db.get<Container::ByIndex>().at(idx).Self());
}

std::size_t EntityVec::IndexOf(const Entity* child) const
{
//	auto it = std::find_if(m_children.begin(), m_children.end(), [child](auto& c){return c.get() == child;});
//	return it != m_children.end() ? static_cast<std::size_t>(it - m_children.begin()) : npos;
	
	auto&& idx = m_db->db.get<Container::BySelf>();
	auto it = idx.find(child);
	if (it != idx.end())
		return mi::project<Container::ByIndex>(m_db->db, it) - m_db->db.get<Container::ByIndex>().begin();
	
	return Entity::npos;
}

void EntityVec::MarkUsed()
{
	m_used = true;
	for (auto& child : m_db->db)
		child.ptr->MarkUsed();
}

bool EntityVec::IsUsed() const
{
	return m_used ||
		std::find_if(m_db->db.begin(), m_db->db.end(), [](auto& child){return child.ptr->IsUsed();}) != m_db->db.end();
}

void EntityVec::MarkSelfUsedOnly()
{
	m_used = true;
}

void EntityVec::Reparent(const Entity *parent)
{
	m_parent = parent;
}

void EntityVec::AddChild(UniqueEntityPtr&& child)
{
//	m_children.push_back(std::move(child));
	m_db->db.insert(std::move(child));
}

const Entity *EntityVec::Find(const std::string& id) const
{
	auto&& idx = m_db->db.get<Container::ByID>();
	auto it = idx.find(id);
	return it != idx.end() ? it->ptr.get() : nullptr;
		
}

bool EntityVec::Modify(const std::string& id, std::function<void(Entity*)> func)
{
	auto&& idx = m_db->db.get<Container::ByID>();
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
