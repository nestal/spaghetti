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
#include "EntityVec.hh"

#include <boost/range/iterator_range_core.hpp>

#include <string>
#include <vector>
#include <iosfwd>

namespace libclx {
class SourceLocation;
}

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
	using field_iterator = EntityVec<Variable>::const_iterator;
	
public:
	DataType(libclx::Cursor cursor);
	DataType(DataType&&) = default;
	DataType(const DataType&) = delete;
	DataType& operator=(DataType&&) = default;
	DataType& operator=(const DataType&) = delete;
	
	const std::string& Name() const override;
	const std::string& USR() const;
	std::string Type() const override;
	const Entity* Parent() const override;
	void Reparent(const Entity *parent) override;
	
	void Visit(EditAction& data, libclx::Cursor self);
	void Merge(EditAction&& data);
	
	boost::iterator_range<field_iterator> Fields() const;

	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	std::size_t IndexOf(const Entity* child) const override;
	
	friend std::ostream& operator<<(std::ostream& os, const DataType& c);
	
	libclx::SourceLocation DefinitionLocation() const;
	
private:
	std::string m_name;
	std::string m_usr;
	
	const Entity *m_parent{};
	
	libclx::SourceLocation m_definition;
	EntityVec<Variable>    m_fields;
	
//	std::vector<DataType> m_nested_classes;
};
	
} // end of namespace
