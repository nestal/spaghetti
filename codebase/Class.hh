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

#include "libclx/Index.hh"

#include <boost/optional.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <string>
#include <vector>
#include <iosfwd>

namespace codebase {

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
		Field(libclx::Cursor field);
		
		const std::string& Name() const;
		
		friend std::ostream& operator<<(std::ostream& os, const Field& c);
		
	private:
		std::string m_name;
		std::string m_usr;
		libclx::Type   m_type;
	};
	
	using FieldList = std::vector<Field>;
	
	class Data
	{
	public:
		Data() = default;
		Data(libclx::Cursor cursor);
		
		friend class Class;
	
	private:
		boost::optional<libclx::SourceLocation> m_definition;
		
		FieldList m_fields;
	};

	using field_iterator = std::vector<Field>::const_iterator;
	
public:
	Class(libclx::Cursor cursor);
	Class(const Class&) = default;
	Class(Class&&) = default;
	
	Class& operator=(const Class&) = default;
	Class& operator=(Class&&) = default;
	
	const std::string& Name() const;
	const std::string& USR() const;
	
	void Visit(Data& data, libclx::Cursor self) const;
	void Merge(Data&& data);
	
	boost::iterator_range<field_iterator> Fields() const;
	std::size_t FieldCount() const;

	friend std::ostream& operator<<(std::ostream& os, const Class& c);
	
private:
	std::string m_name;
	std::string m_usr;
	
	Data m_data;
};
	
} // end of namespace
