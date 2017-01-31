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
#include <vector>

namespace cb {

class CppClass
{
public:
	CppClass(clx::Cursor cursor);
	CppClass(const CppClass&) = default;
	CppClass(CppClass&&) = default;
	
	CppClass& operator=(const CppClass&) = default;
	CppClass& operator=(CppClass&&) = default;
	
	void Visit(clx::Cursor cursor, clx::Cursor parent);
	
	void AddMemberFunction(const std::string& name);
	void AddDataMember(const std::string& name);
	
	const std::string& Name() const;
	const std::string& USR() const;
	
private:
	std::string m_name;
	std::string m_usr;
	
	clx::Type m_type;
	
	std::vector<std::string> m_field_usr;
	std::vector<std::string> m_func_usr;
};
	
} // end of namespace
