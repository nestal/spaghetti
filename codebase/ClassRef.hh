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
	explicit ClassRef(String&& base) :
		m_base_id{std::forward<String>(base)}
	{
	}
	
	template <typename String>
	ClassRef(String&& base, String&& temp, std::initializer_list<std::string> args) :
		m_base_id{std::forward<String>(base)},
		m_temp_id{std::forward<String>(temp)},
		m_temp_args{args}
	{
	}
	
	const std::string& ID() const;
	void SetID(std::string&& base_id);
	
	const std::string& TemplateID() const;
	void SetTemplateID(const std::string&& id);
	
	const std::vector<std::string>& TempArgs() const;
	void AddTempArgs(std::string&& arg);
	
	bool IsTemplate() const;
	
	bool operator==(const ClassRef& ref) const;
	bool operator!=(const ClassRef& ref) const;
	
private:
	//! USR of the class or template instantiation
	std::string m_base_id;
	
	//! USR of the class template that instantiate the class, or empty if non-template
	std::string m_temp_id;
	
	//! USR of template arguments, empty if non-template
	std::vector<std::string> m_temp_args;
};

std::ostream& operator<<(std::ostream& os, const ClassRef& ref);
	
} // end of namespace
