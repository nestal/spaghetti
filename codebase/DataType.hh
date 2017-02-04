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

#include "Entity.hh"
#include "Variable.hh"

#include <boost/optional.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <string>
#include <vector>
#include <iosfwd>

namespace codebase {

class EditAction;

/**
 * \brief Represent a C++ class/struct in the CodeBase.
 *
 * This class represent particular data type in the code base. A data type is an
 * entity that can be used to definite data in C++.
 */
class DataType : public Entity
{
public:
	using FieldList = std::vector<Variable>;
	using field_iterator = FieldList::const_iterator;
	
public:
	DataType(libclx::Cursor cursor, const Entity *parent);
	DataType(const DataType&) = default;
	DataType(DataType&&) = default;
	
	DataType& operator=(const DataType&) = default;
	DataType& operator=(DataType&&) = default;
	
	const std::string& Name() const override;
	const std::string& USR() const;
	std::string Type() const override;
	const Entity* Parent() const override;
	
	void Visit(EditAction& data, libclx::Cursor self) const;
	void Merge(EditAction&& data);
	
	boost::iterator_range<field_iterator> Fields() const;

	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	std::size_t IndexOf(const Entity* child) const override;
	
	friend std::ostream& operator<<(std::ostream& os, const DataType& c);
	
private:
	std::string m_name;
	std::string m_usr;
	
	const Entity *m_parent{};
	
	boost::optional<libclx::SourceLocation> m_definition;
	FieldList m_fields;
	
	std::vector<DataType> m_nested_classes;
};
	
} // end of namespace
