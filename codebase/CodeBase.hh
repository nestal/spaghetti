/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
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

#include "libclx/Index.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>

#include <vector>

namespace codebase {

class CodeBase : public Entity, public TypeDB
{
public:
/*	struct ByUSR {};
	struct ByIndex {};
	struct ByLocation {};
	
	using ClassDB = boost::multi_index_container<
		DataType,
		boost::multi_index::indexed_by<
			
			boost::multi_index::random_access<
				boost::multi_index::tag<ByIndex>
			>,
			
			// hash by USR
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<ByUSR>,
				boost::multi_index::const_mem_fun<
					DataType,
					const std::string&,
					&DataType::USR
				>
			>,
			
			// hash by SourceLocation
			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<ByLocation>,
				boost::multi_index::const_mem_fun<
					DataType,
					libclx::SourceLocation,
					&DataType::DefinitionLocation
				>,
				libclx::SourceLocation::Hash
			>
		>
	>;
*/
public:
	CodeBase() = default;
	
	std::string Parse(const std::string& source);
	
	void Visit(libclx::Cursor cursor, libclx::Cursor parent);

	const std::string& Name() const override;
	const Entity* Parent() const override;
	std::string Type() const override;
	
	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	std::size_t IndexOf(const Entity* child) const override;
	
	const DataType* Find(const SourceLocation& loc) const override;
	void Add(const DataType *type, const SourceLocation& loc) override;

private:
	libclx::Index  m_index;
	std::vector<libclx::TranslationUnit> m_units;
	
//	ClassDB m_classes;
	EntityVec<DataType> m_types{nullptr, "Data Types"};
};

} // end of namespace
