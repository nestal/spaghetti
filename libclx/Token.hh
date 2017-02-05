/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#pragma once

#include <clang-c/Index.h>

#include <boost/iterator/iterator_facade.hpp>

namespace libclx {

class TokenSet
{
public:
	TokenSet(CXTranslationUnit tu, CXSourceRange range);
	TokenSet(const TokenSet&) = delete;
	TokenSet(TokenSet&&) = default;
	TokenSet& operator=(const TokenSet&) = delete;
	TokenSet& operator=(TokenSet&&) = default;
	~TokenSet();
	
	std::size_t size() const;
	const CXToken& at(std::size_t idx) const;
	
	using iterator = const CXToken*;
	
	iterator begin() const;
	iterator end() const;
	
private:
	CXTranslationUnit m_tu{};
	CXToken *m_set{};
	unsigned m_count{};
};
	
} // end of namespace
