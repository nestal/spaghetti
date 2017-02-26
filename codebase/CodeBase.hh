/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#pragma once

#include "Entity.hh"

#include "Namespace.hh"

#include "libclx/Index.hh"

#include <boost/optional.hpp>
#include <boost/range.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace codebase {

/**
 * \brief The root of the Entity tree in a code base.
 */
class CodeBase
{
public:
	using iterator = std::vector<libclx::TranslationUnit>::const_iterator;
	
public:
	CodeBase();
	~CodeBase();
	
	std::string Parse(const std::string& source, const std::vector<std::string>& ops);
	void ReparseAll(std::function<void(const EntityMap*)> callback = {});
	
	const Entity* Root() const;
	const EntityMap& Map() const;
	EntityMap& Map();

	boost::optional<const libclx::TranslationUnit&> Locate(const libclx::SourceLocation& loc) const;
	
	boost::iterator_range<iterator> TranslationUnits() const;
	const libclx::TranslationUnit& At(std::size_t idx) const;
	std::size_t Size() const;
	
private:
	class SearchIndex;
	
	void BuildEntityTree(libclx::TranslationUnit& tu, Namespace& root, SearchIndex& map);
	void AddToIndex(Entity *entity, SearchIndex& map) ;
	void CrossReference(Entity *entity, SearchIndex& map) ;

private:
	libclx::Index  m_index;
	std::vector<libclx::TranslationUnit> m_units;
	
	Namespace    m_root;
	std::unique_ptr<SearchIndex> m_search_index;
};

} // end of namespace
