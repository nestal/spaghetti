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
	class Field
	{
	public:
		Field(clx::Cursor field);
		
		const std::string& Name() const;
	
	private:
		std::string m_name;
		std::string m_usr;
		clx::Type   m_type;
	};
	
	using FieldList = std::vector<Field>;
	
	class Data
	{
	public:
		Data() = default;
		Data(clx::Cursor cursor);
		
		friend class Class;
	
	private:
		boost::optional<clx::SourceLocation> m_definition;
		
		FieldList m_fields;
	};

	using field_iterator = std::vector<Field>::const_iterator;
	
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
	
	void Visit(Data& data, clx::Cursor self) const;
	void Merge(Data&& data);
	
	std::pair<field_iterator, field_iterator> Fields() const;

private:
	std::string m_name;
	std::string m_usr;
	
	Data m_data;
};
	
} // end of namespace
