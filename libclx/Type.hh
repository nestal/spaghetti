/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#pragma once

#include <clang-c/Index.h>
#include <string>
#include <iosfwd>

namespace libclx {

class Cursor;

class Type
{
public:
	Type(CXType type);
	
	std::string Spelling() const;
	Cursor Declaration() const;
	
	std::string Kind() const;
	Type ClassType() const;
	int NumTemplateArguments() const;
	
	friend std::ostream& operator<<(std::ostream& os, const Type& t);

private:
	CXType m_type;
};

} // end of namespace
