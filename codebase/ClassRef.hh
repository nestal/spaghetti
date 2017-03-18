/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#pragma once

#include <vector>
#include <string>
#include <iosfwd>

namespace codebase {

class ClassRef
{
public:
	ClassRef() = default;
	
	template <typename String>
	ClassRef(String&& base, std::initializer_list<std::string> args = {}) :
		m_base_id{std::forward<String>(base)},
		m_temp_args{args}
	{
	}
	
	const std::string& BaseID() const;
	void SetBaseID(std::string&& base_id);
	
	const std::vector<std::string>& TempArgs() const;
	void AddTempArgs(std::string&& arg);
	
	bool operator==(const ClassRef& ref) const;
	bool operator!=(const ClassRef& ref) const;
	
private:
	//! USR of the class or class template
	std::string m_base_id;
	
	//! USR of template arguments, empty if non-template
	std::vector<std::string> m_temp_args;
};

std::ostream& operator<<(std::ostream& os, const ClassRef& ref);
	
} // end of namespace
