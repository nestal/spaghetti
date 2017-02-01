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

#include "Class.hh"
#include "libclx/Index.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <vector>

namespace codebase {

class CodeBase
{
public:
	struct ByUSR {};
	
	using ClassDB = boost::multi_index_container<
		Class,
		boost::multi_index::indexed_by<
			
			// hash by USR
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<ByUSR>,
				boost::multi_index::const_mem_fun<
					Class,
					const std::string&,
					&Class::USR
				>
			>
		>
	>;
	
	using USRIndex = ClassDB::index<ByUSR>::type ;
	using usr_iterator = USRIndex::iterator;

public:
	CodeBase() = default;
	
	std::string Parse(const std::string& source);
	
	void Visit(libclx::Cursor cursor, libclx::Cursor parent);
	
	usr_iterator begin() const;
	usr_iterator end() const;
	usr_iterator find(const std::string& usr) const;
	
private:
	libclx::Index  m_index;
	std::vector<libclx::TranslationUnit> m_units;
	
	ClassDB m_classes;
};
	
} // end of namespace
