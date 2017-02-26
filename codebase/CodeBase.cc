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

namespace codebase {

class CodeBase::MapByIndex : public EntityMap
{
public:
	MapByIndex(CodeBase::EntityIndex& index) : m_index{index} {}
	
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

private:
	CodeBase::EntityIndex& m_index;
};

CodeBase::CodeBase()
{
	MapByIndex map{m_search_index};
	AddToIndex(&m_root, map);
}

std::string CodeBase::Parse(const std::string& source, const std::vector<std::string>& ops)
{
	auto tu = m_index.Parse(source, ops, CXTranslationUnit_SkipFunctionBodies);
	
	m_search_index.clear();
	
	MapByIndex map{m_search_index};
	BuildEntityTree(tu, m_root, map);
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

void CodeBase::ReparseAll()
{
	// build a new tree and replace our own with it
	Namespace   new_root;
	EntityIndex new_index;
	MapByIndex  map{new_index};
	
	for (auto&& tu : m_units)
		BuildEntityTree(tu, new_root, map);
	
	// replace our own with the new one
	m_root = std::move(new_root);
	m_search_index = new_index;
}

void CodeBase::BuildEntityTree(libclx::TranslationUnit& tu, Namespace& root, MapByIndex& map)
{
	root.Visit(tu.Root());
	AddToIndex(&root, map);
	CrossReference(&root, map);
}

void CodeBase::AddToIndex(Entity *entity, MapByIndex& map)
{
	map.Insert(entity);
	
	for (auto&& c : *entity)
		AddToIndex(&c, map);
}

void CodeBase::CrossReference(Entity *entity, MapByIndex& map)
{
	entity->CrossReference(&map);
	for (auto&& child : *entity)
		CrossReference(&child, map);
}

const Entity *CodeBase::Root() const
{
	return &m_root;
}

boost::optional<const libclx::TranslationUnit&> CodeBase::Locate(const SourceLocation& loc) const
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

const Entity *CodeBase::Find(const std::string& id) const
{
	auto it = m_search_index.get<ByID>().find(id);
	return it != m_search_index.get<ByID>().end() ? *it : nullptr;
}

Entity *CodeBase::Find(const std::string& id)
{
	auto it = m_search_index.get<ByID>().find(id);
	return it != m_search_index.get<ByID>().end() ? *it : nullptr;
}

	
} // end of namespace
