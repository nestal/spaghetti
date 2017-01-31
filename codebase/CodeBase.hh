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

#include <vector>

namespace clx {
class Cursor;
}

namespace cb {

class CppClass;
class CodeBase
{
public:
	CodeBase() = default;
	
	void Parse(const std::string& source);
	
	void Visit(clx::Cursor cursor, clx::Cursor parent);
	
private:
	clx::Index  m_index;
	std::vector<clx::TranslationUnit> m_units;
	
	std::vector<CppClass> m_classes;
};
	
} // end of namespace
