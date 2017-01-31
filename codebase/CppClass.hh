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

#include "libclangxx/Index.hh"
#include <string>

namespace cb {

class CppClass
{
public:
	CppClass(clx::Cursor cursor);
	CppClass(const CppClass&) = default;
	CppClass(CppClass&&) = default;
	
	void AddMemberFunction(const std::string& name);
	void AddDataMember(const std::string& name);
	
	std::string Name() const;
	clx::Cursor Cursor() const;
	
private:
	clx::Cursor m_cursor;
};
	
} // end of namespace
