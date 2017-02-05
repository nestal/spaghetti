/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/30/17.
//

#pragma once

#include <clang-c/CXString.h>

#include <string>

namespace libclx {

class XStr
{
public:
	XStr(CXString s);
	XStr(XStr&&) = default;
	~XStr();
	
	std::string Str() const;
	
private:
	CXString m_str;
};
	
} // end of namespace
