/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//


#pragma once

#include "EntityVec.hh"
#include "libclx/SourceRange.hh"

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <string>
#include <vector>
#include <iosfwd>

namespace codebase {

class Function;
class Variable;

/**
 * \brief Represent a C++ class/struct in the CodeBase.
 *
 * This class represent particular data type in the code base. A data type is an
 * entity that can be used to definite data in C++.
 */
class DataType : public EntityVec
{
public:
	using field_iterator = boost::indirect_iterator<std::vector<Variable*>::const_iterator>;
	using idvec_iterator = std::vector<std::string>::const_iterator;
	
public:
	DataType(libclx::Cursor cursor, const Entity* parent);
	DataType(DataType&&) = default;
	DataType(const DataType&) = delete;
	DataType& operator=(DataType&&) = default;
	DataType& operator=(const DataType&) = delete;
	
	std::string Type() const override;
	
	libclx::SourceLocation Location() const override;
	
	void Visit(libclx::Cursor self);
	void VisitFunction(libclx::Cursor func);
	
	boost::iterator_range<field_iterator> Fields() const;
	boost::iterator_range<idvec_iterator> BaseClasses() const;

	bool IsBaseOf(const DataType& other) const;
	
	friend std::ostream& operator<<(std::ostream& os, const DataType& c);

	void RemoveUnused() override;
	
private:
	libclx::SourceLocation   m_definition;
	std::vector<std::string> m_base_classes;
	std::vector<Variable*>   m_fields;
	std::vector<Function*>   m_functions;
};
	
} // end of namespace
