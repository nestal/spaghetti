/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "Token.hh"

namespace libclx {

TokenSet::TokenSet(CXTranslationUnit tu, CXSourceRange range) :
	m_tu{tu}
{
	::clang_tokenize(m_tu, range, &m_set, &m_count);
}

TokenSet::~TokenSet()
{
	::clang_disposeTokens(m_tu, m_set, m_count);
}

std::size_t TokenSet::size() const
{
	return m_count;
}

const CXToken& TokenSet::at(std::size_t idx) const
{
	return m_set[idx];
}

TokenSet::iterator TokenSet::begin() const
{
	return &m_set[0];
}

TokenSet::iterator TokenSet::end() const
{
	return &m_set[m_count];
}
	
} // end of namespace
