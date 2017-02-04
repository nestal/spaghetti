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
 * \brief Represent a C++ class in the CodeBase.
 *
 * This class represent particular class in the code base. There will be only one copy of
 * object for each class in the code base, regardless of how many translation unit uses it.
 */
class Class : public Entity
{
public:
	using FieldList = std::vector<Variable>;
	
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

	using field_iterator = FieldList::const_iterator;
	
public:
	Class(libclx::Cursor cursor, const Entity *parent);
	Class(const Class&) = default;
	Class(Class&&) = default;
	
	Class& operator=(const Class&) = default;
	Class& operator=(Class&&) = default;
	
	const std::string& Name() const override;
	const std::string& USR() const;
	const Entity* Parent() const override;
	
	void Visit(EditAction& data, libclx::Cursor self) const;
	void Merge(EditAction&& data);
	
	boost::iterator_range<field_iterator> Fields() const;

	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	std::size_t IndexOf(const Entity* child) const override;
	
	friend std::ostream& operator<<(std::ostream& os, const Class& c);
	
private:
	std::string m_name;
	std::string m_usr;
	
	const Entity *m_parent{};
	
	Data m_data;
	
	std::vector<Class> m_nested_classes;
};
	
} // end of namespace
