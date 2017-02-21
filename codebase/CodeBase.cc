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

#include <iostream>

namespace codebase {

CodeBase::CodeBase()
{
	AddToIndex(&m_root);
}

std::string CodeBase::Parse(const std::string& source, const std::vector<std::string>& ops)
{
	auto tu = m_index.Parse(source, ops, CXTranslationUnit_SkipFunctionBodies);
	
	m_search_index.clear();
	m_root.Visit(tu.Root());
	AddToIndex(&m_root);
	
	for (auto&& diag : tu.Diagnostics())
		std::cerr << diag.Str() << "\n";
	
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

const Entity *CodeBase::Find(const std::string& id) const
{
	auto it = m_search_index.get<ByID>().find(id);
	return it != m_search_index.get<ByID>().end() ? *it : nullptr;
}

void CodeBase::AddToIndex(const Entity *entity)
{
	m_search_index.insert(entity);
	
	for (auto&& c : *entity)
		AddToIndex(&c);
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
	
} // end of namespace
