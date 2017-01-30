/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/28/17.
//

#include "Index.hh"
#include "XStr.hh"

#include <vector>

namespace clx {

Index::Index() :
	m_index{::clang_createIndex(0, 0)}
{
	
}

TranslationUnit Index::Parse(std::initializer_list<std::string> args, unsigned options)
{
	std::vector<const char*> vargs;
	for (auto&& arg : args)
		vargs.push_back(arg.c_str());
	
	return {::clang_parseTranslationUnit(m_index.get(), 0, &vargs[0], static_cast<int>(vargs.size()), 0, 0, options)};
}

CXIndex Index::Get()
{
	return m_index.get();
}

TranslationUnit::TranslationUnit(CXTranslationUnit tu) :
	m_unit{tu}
{
}

CXTranslationUnit TranslationUnit::Get()
{
	return m_unit.get();
}

std::string TranslationUnit::Spelling() const
{
	return XStr{::clang_getTranslationUnitSpelling(m_unit.get())}.Str();
}
	
} // end of namespace
