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

#include "CppClass.hh"
#include "libclangxx/Index.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <vector>

namespace clx {
class Cursor;
}

namespace cb {

class CodeBase
{
public:
	CodeBase() = default;
	
	void Parse(const std::string& source);
	
	void Visit(clx::Cursor cursor, clx::Cursor parent);
	
	const CppClass* FindClass(clx::Cursor cursor) const;
	
private:
	clx::Index  m_index;
	std::vector<clx::TranslationUnit> m_units;
	
	struct ByCursor {};
	struct ByUSR {};
	
	boost::multi_index_container<
		CppClass,
		boost::multi_index::indexed_by<
			
			// hash by cursor
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<ByCursor>,
				boost::multi_index::const_mem_fun<
					CppClass,
					clx::Cursor,
					&CppClass::Cursor
				>,
				clx::Cursor::Hash
			>,
			    
			// hash by USR
			boost::multi_index::hashed_unique<
			    boost::multi_index::tag<ByUSR>,
			    boost::multi_index::const_mem_fun<
			        CppClass,
			        std::string,
			        &CppClass::USR
				>
			>
		>
	> m_classes;
};
	
} // end of namespace
