/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/30/17.
//

#include "XStr.hh"

namespace clx {

XStr::XStr(CXString s) :
	m_str{s}
{
}

XStr::~XStr()
{
	::clang_disposeString(m_str);
}

std::string XStr::Str() const
{
	return {clang_getCString(m_str)};
}
	
}