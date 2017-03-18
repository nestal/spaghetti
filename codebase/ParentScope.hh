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

#include "EntityVec.hh"
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
class DataType;
class ClassTemplate;
class ClassRef;

/**
 * \brief Base class of Namespace, Class, Structure and Union
 */
class ParentScope : public EntityVec
{
public:
	using field_iterator    = boost::indirect_iterator<std::vector<codebase::Variable*>::const_iterator>;
	using function_iterator = boost::indirect_iterator<std::vector<codebase::Function*>::const_iterator>;
	using idvec_iterator    = std::vector<ClassRef>::const_iterator;

public:
	ParentScope(const libclx::Cursor& cursor, const Entity* parent);
	ParentScope(const std::string& name, const std::string& usr, const Entity *parent);
	ParentScope(ParentScope&&) = default;
	ParentScope(const ParentScope&) = delete;
	ParentScope& operator=(ParentScope&&) = default;
	ParentScope& operator=(const ParentScope&) = delete;
	
	void Visit(const libclx::Cursor& self);
	
	boost::iterator_range<field_iterator> Fields() const;
	boost::iterator_range<function_iterator> Functions() const;
	
	const codebase::Function& Function(std::size_t idx) const;
	const codebase::Variable& Field(std::size_t idx) const;
	
	void VisitFunction(const libclx::Cursor& func);
	
	void Add(std::unique_ptr<DataType>&& type);
	
protected:
	virtual void VisitChild(const libclx::Cursor& child, const libclx::Cursor& self);
	virtual void OnVisit(const libclx::Cursor& self);
	virtual void AfterVisitingChild(const libclx::Cursor& self);
	
	std::vector<DataType*>& Types();
	
private:
	std::vector<Variable*>      m_fields;
	std::vector<codebase::Function*>      m_functions;
	std::vector<DataType*>      m_types;
	std::vector<ClassTemplate*> m_temps;
};

} // end of namespace
