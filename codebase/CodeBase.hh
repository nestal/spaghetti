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
#include "TypeDB.hh"

#include "DataType.hh"
#include "EntityVec.hh"
#include "Namespace.hh"

#include "libclx/Index.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>

#include <boost/optional.hpp>
#include <boost/range.hpp>

#include <vector>

namespace codebase {

/**
 * \brief The root of the Entity tree in a code base.
 */
class CodeBase : public EntityMap
{
public:
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

	using iterator = std::vector<libclx::TranslationUnit>::const_iterator;
	
public:
	CodeBase();
	
	std::string Parse(const std::string& source, const std::vector<std::string>& ops);
	
	const Entity* Root() const;
	const Entity* Find(const std::string& id) const override;
	Entity* Find(const std::string& id) override;

	boost::optional<const libclx::TranslationUnit&> Locate(const libclx::SourceLocation& loc) const;
	
	boost::iterator_range<iterator> TranslationUnits() const;
	const libclx::TranslationUnit& At(std::size_t idx) const;
	std::size_t Size() const;
	
private:
	void AddToIndex(Entity *entity) ;

private:
	libclx::Index  m_index;
	std::vector<libclx::TranslationUnit> m_units;
	
	Namespace    m_root;
	EntityIndex  m_search_index;
};

} // end of namespace
