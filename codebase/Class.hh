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

#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace cb {

/**
 * \brief Represent a C++ class in the CodeBase.
 *
 * This class represent particular class in the code base. There will be only one copy of
 * object for each class in the code base, regardless of how many translation unit uses it.
 */
class Class
{
public:
	Class(clx::Cursor cursor);
	Class(const Class&) = default;
	Class(Class&&) = default;
	
	Class& operator=(const Class&) = default;
	Class& operator=(Class&&) = default;
	
	void AddMemberFunction(const std::string& name);
	void AddDataMember(const std::string& name);
	
	const std::string& Name() const;
	const std::string& USR() const;
	
	class Data
	{
	public:
		Data() = default;
		Data(clx::Cursor cursor);
		
		friend class Class;
		
	private:
		boost::optional<clx::SourceLocation> m_definition;
		
		std::vector<std::string> m_field_usr;
		std::vector<std::string> m_func_usr;
	};
	
	void Visit(Data& data, clx::Cursor self) const;
	void Merge(Data&& data);

private:
	std::string m_name;
	std::string m_usr;
	
	Data m_data;
};
	
} // end of namespace
