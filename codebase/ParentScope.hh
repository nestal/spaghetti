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

#include "util/MultiContainer.hh"
#include "libclx/SourceRange.hh"

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
class Namespace;
class ClassRef;

/**
 * \brief Base class of Namespace, Class, Structure and Union
 */
class ParentScope : public EntityVec
{
public:
	using field_iterator            = std::vector<codebase::Variable>::const_iterator;
	using function_iterator         = std::vector<codebase::Function>::const_iterator;
	using class_template_iterator   = std::vector<codebase::ClassTemplate>::const_iterator;
	using data_type_iterator        = std::vector<codebase::DataType>::const_iterator;
	using idvec_iterator            = std::vector<ClassRef>::const_iterator;

public:
	ParentScope(const libclx::Cursor& cursor, const EntityVec* parent);
	ParentScope(const std::string& name, const std::string& usr, const EntityVec *parent);
	ParentScope(ParentScope&&);
	ParentScope(const ParentScope&) = delete;
	ParentScope& operator=(ParentScope&&);
	ParentScope& operator=(const ParentScope&) = delete;
	~ParentScope();
	
	void Visit(const libclx::Cursor& self);
	
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;
	
	boost::iterator_range<field_iterator> Fields() const;
	boost::iterator_range<function_iterator> Functions() const;
	boost::iterator_range<class_template_iterator> ClassTemplates() const;
	boost::iterator_range<data_type_iterator> DataTypes() const;
	
	const codebase::Function& Function(std::size_t idx) const;
	const codebase::Variable& Field(std::size_t idx) const;
	
	void VisitFunction(const libclx::Cursor& func);
	
protected:
	virtual void VisitChild(const libclx::Cursor& child, const libclx::Cursor& self);
	virtual void OnVisit(const libclx::Cursor& self);
	virtual void AfterVisitingChild(const libclx::Cursor& self);
	
protected:
	using Cond = util::MultiContainer<
		Entity,
		Variable,
		codebase::Function,
		DataType,
		ClassTemplate,
		    
		// will remove later
		Namespace
	>;
	std::unique_ptr<Cond> m_cond;
};

} // end of namespace
