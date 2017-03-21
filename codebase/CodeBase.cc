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

#include "EntityMap.hh"
#include "Namespace.hh"
#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace codebase {

namespace mi = boost::multi_index;

class CodeBase::EntityTree : public EntityMap
{
public:
	EntityTree()
	{
		AddToIndex(&m_root);
	}
	
	const Entity* Find(const std::string& id) const override
	{
		auto it = m_index.get<ByID>().find(id);
		return it != m_index.get<ByID>().end() ? *it : nullptr;
	}
	
	const Entity* FindByName(const std::string& name) const override
	{
		auto it = m_index.get<ByName>().find(name);
		return it != m_index.get<ByName>().end() ? *it : nullptr;
	}
	
	const DataType* Find(const ClassRef& ref) const override
	{
		return dynamic_cast<const DataType*>(Find(ref.ID()));
	}
	
	const DataType* Instantiate(const ClassRef& ref) override
	{
		assert(ref.IsTemplate());
		assert(!ref.ID().empty());
		
		auto result = Find(ref);
		if (!result)
		{
			if (auto temp = dynamic_cast<const ClassTemplate*>(Find(ref.TemplateID())))
			{
				// instantiate template and add to the index immediately
				auto inst = temp->Instantiate(ref);
				AddToIndex(inst.get());
				
				auto parent = dynamic_cast<const ParentScope *>(Find(temp->Parent()->ID()));
				
				assert(parent);
				assert(inst);
				
				result = inst.get();
				const_cast<ParentScope*>(parent)->Add(std::move(inst));
			}
		}
		
		return result;
	}

	Entity* Root()
	{
		return &m_root;
	}
	
	void Build(libclx::TranslationUnit& tu)
	{
		m_index.clear();
		m_root.Visit(tu.Root());
		AddToIndex(&m_root);
		CrossReference(&m_root);
	}
	
	void AddToIndex(const Entity *entity)
	{
		m_index.insert(entity);
		
		for (auto&& c : *entity)
			AddToIndex(&c);
	}
	
	void CrossReference(const Entity *entity)
	{
		const_cast<Entity*>(entity)->CrossReference(this);

		for (auto&& child : *entity)
			CrossReference(&child);
	}
	
	void Swap(EntityTree& other)
	{
		using namespace std;
		swap(m_index, other.m_index);
		swap(m_root, other.m_root);
	}

private:
	struct ByID {};
	struct ByName {};
	
	using EntityIndex = boost::multi_index_container<
		const Entity*,
		mi::indexed_by<
			
			// hash by ID
			mi::hashed_unique<
				mi::tag<ByID>,
				mi::const_mem_fun<
					Entity,
					const std::string&,
					&Entity::ID
				>
			>,
			
			mi::hashed_non_unique<
			    mi::tag<ByName>,
			    mi::const_mem_fun<
			        Entity,
				    const std::string&,
				    &Entity::Name
			    >
			>
		>
	>;

private:
	Namespace   m_root;
	EntityIndex m_index;
};

CodeBase::CodeBase() :
	m_tree{std::make_unique<EntityTree>()}
{
}

CodeBase::~CodeBase() = default;

std::string CodeBase::Parse(const std::string& source, const std::vector<std::string>& ops)
{
	auto tu = m_index.Parse(source, ops, CXTranslationUnit_SkipFunctionBodies);
	
	m_tree->Build(tu);
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

void CodeBase::ReparseAll(std::function<void(const EntityMap*, const Entity*)> callback)
{
	// build a new tree and replace our own with it
	auto tree = std::make_unique<EntityTree>();
	
	for (auto&& tu : m_units)
	{
		tu.Reparse();
		tree->Build(tu);
	}
	
	// replace our own with the new one
	// use Swap() to avoid dangling the references returned by Map()
	m_tree->Swap(*tree);
	
	// before the old root and search index are invalidate, notify
	// the caller to update their data structures
	callback(m_tree.get(), m_tree->Root());
}

const Entity *CodeBase::Root() const
{
	return m_tree->Root();
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
	return *m_tree.get();
}

EntityMap& CodeBase::Map()
{
	return *m_tree.get();
}
	
} // end of namespace
