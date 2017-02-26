/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "CodeBase.hh"

#include "libclx/Cursor.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace codebase {

class CodeBase::SearchIndex : public EntityMap
{
public:
	SearchIndex() = default;
	
	const Entity* Find(const std::string& id) const override
	{
		auto it = m_index.get<ByID>().find(id);
		return it != m_index.get<ByID>().end() ? *it : nullptr;
	}
	
	Entity* Find(const std::string& id) override
	{
		auto it = m_index.get<ByID>().find(id);
		return it != m_index.get<ByID>().end() ? *it : nullptr;
	}
	
	void Insert(Entity *e)
	{
		m_index.insert(e);
	}
	
	void Clear()
	{
		m_index.clear();
	}
	
	void Swap(SearchIndex& other)
	{
		using namespace std;
		swap(m_index, other.m_index);
	}

private:
	struct ByID {};
	
	using EntityIndex = boost::multi_index_container<
		Entity*,
		boost::multi_index::indexed_by<
			
			// hash by ID
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<ByID>,
				boost::multi_index::const_mem_fun<
					Entity,
					const std::string&,
					&Entity::ID
				>
			>
		>
	>;

private:
	EntityIndex m_index;
};

CodeBase::CodeBase() : m_search_index{std::make_unique<SearchIndex>()}
{
	AddToIndex(&m_root, *m_search_index);
}

CodeBase::~CodeBase() = default;

std::string CodeBase::Parse(const std::string& source, const std::vector<std::string>& ops)
{
	auto tu = m_index.Parse(source, ops, CXTranslationUnit_SkipFunctionBodies);
	
	m_search_index->Clear();
	
	BuildEntityTree(tu, m_root, *m_search_index);
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

void CodeBase::ReparseAll(std::function<void(const EntityMap*)> callback)
{
	// build a new tree and replace our own with it
	Namespace   new_root;
	auto map = std::make_unique<SearchIndex>();
	
	for (auto&& tu : m_units)
		BuildEntityTree(tu, new_root, *map);
	
	// replace our own with the new one
	// use Swap() to avoid dangling the references returned by Map()
	m_root         = std::move(new_root);
	m_search_index->Swap(*map);
	
	// before the old root and search index are invalidate, notify
	// the caller to update their data structures
	callback(m_search_index.get());
}

void CodeBase::BuildEntityTree(libclx::TranslationUnit& tu, Namespace& root, SearchIndex& map)
{
	root.Visit(tu.Root());
	AddToIndex(&root, map);
	CrossReference(&root, map);
}

void CodeBase::AddToIndex(Entity *entity, SearchIndex& map)
{
	map.Insert(entity);
	
	for (auto&& c : *entity)
		AddToIndex(&c, map);
}

void CodeBase::CrossReference(Entity *entity, SearchIndex& map)
{
	entity->CrossReference(&map);
	for (auto&& child : *entity)
		CrossReference(&child, map);
}

const Entity *CodeBase::Root() const
{
	return &m_root;
}

boost::optional<const libclx::TranslationUnit&> CodeBase::Locate(const libclx::SourceLocation& loc) const
{
	for (auto&& c : m_units)
	{
		auto cursor = c.Locate(loc);
		if (cursor)
			return c;
	}
	return {};
}

boost::iterator_range<CodeBase::iterator> CodeBase::TranslationUnits() const
{
	return {m_units.begin(), m_units.end()};
}

const libclx::TranslationUnit& CodeBase::At(std::size_t idx) const
{
	return m_units.at(idx);
}

std::size_t CodeBase::Size() const
{
	return m_units.size();
}

const EntityMap& CodeBase::Map() const
{
	return *m_search_index.get();
}

EntityMap& CodeBase::Map()
{
	return *m_search_index.get();
}
	
} // end of namespace
