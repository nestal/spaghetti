/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/28/17.
//

#pragma once

#include "DeleteWith.hh"

#include <clang-c/Index.h>

namespace clx {

class TranslationUnit;

class Index
{
public:
	Index();
	Index(Index&&) = default;
	
	TranslationUnit Parse(std::initializer_list<std::string> args, unsigned options);
	
	CXIndex Get();
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXIndex>,
		void,
		&::clang_disposeIndex
	> m_index;
};

class TranslationUnit
{
public:
	TranslationUnit(CXTranslationUnit tu);
	TranslationUnit(TranslationUnit&&) = default;
	
	CXTranslationUnit Get();
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXTranslationUnit>,
		void,
		&::clang_disposeTranslationUnit
	> m_unit;
};

} // end of namespace clx
