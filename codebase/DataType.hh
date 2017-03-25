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

#include "ParentScope.hh"
#include "ClassRef.hh"
#include "libclx/SourceRange.hh"

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <string>
#include <vector>
#include <iosfwd>

namespace libclx {
class Cursor;
}

namespace codebase {

class Function;
class Variable;

/**
 * \brief Represent a C++ class/struct in the CodeBase.
 *
 * This class represent particular data type in the code base. A data type is an
 * entity that can be used to definite data in C++.
 */
class DataType : public ParentScope
{
	using idvec_iterator    = std::vector<ClassRef>::const_iterator;
	
public:
	DataType(const libclx::Cursor& cursor, const EntityVec* parent);
	DataType(DataType&&) = delete;
	DataType(const DataType&) = delete;
	DataType& operator=(DataType&&) = delete;
	DataType& operator=(const DataType&) = delete;
	
	EntityType Type() const override;
	
	libclx::SourceLocation Location() const override;
	
	boost::iterator_range<idvec_iterator> BaseClasses() const;

	bool IsBaseOf(const DataType& other) const;
	bool IsUsedInMember(const DataType& other) const;
	
	friend std::ostream& operator<<(std::ostream& os, const DataType& c);

	void CrossReference(EntityMap *map) override;

private:
	void MarkBaseClassUsed(EntityMap *map);

protected:
	DataType(
		const std::string& name,
		const std::string& usr,
		const libclx::SourceLocation def,
		const EntityVec *parent
	);
	
	void AddBase(const ClassRef& base);
	void AddField(const Variable& var);
	
	void OnVisit(const libclx::Cursor& self) override;
	void VisitChild(const libclx::Cursor& child, const libclx::Cursor& self) override;
	
private:
	libclx::SourceLocation              m_definition;
	std::vector<ClassRef>               m_bases;
};

} // end of namespace
