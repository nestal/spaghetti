/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "DataType.hh"

#include <iostream>

namespace codebase {

DataType::DataType(libclx::Cursor cursor, const std::string& parent) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_parent{parent},
	m_fields{"Fields", m_usr}
{
	assert(cursor.Kind() == CXCursor_StructDecl || cursor.Kind() == CXCursor_ClassDecl);
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}

const std::string& DataType::Name() const
{
	return m_name;
}

const std::string& DataType::ID() const
{
	return m_usr;
}

void DataType::Visit(libclx::Cursor self)
{
	assert(self.Kind() == CXCursor_StructDecl || self.Kind() == CXCursor_ClassDecl);
	assert(m_name == self.Spelling());
	assert(!m_usr.empty() && m_usr == self.USR());
	
	if (self.IsDefinition() || m_definition == libclx::SourceLocation{})
		m_definition = self.Location();
	
	self.Visit([this](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_FieldDecl:
			m_fields.Add(child, m_fields.ID());
			break;
	
		default:
//		std::cout << child.Spelling() << ' ' << child.Kind() << std::endl;
			break;
		}
	});
}

boost::iterator_range<DataType::field_iterator> DataType::Fields() const
{
	return {m_fields.begin(), m_fields.end()};
}

const std::string& DataType::Parent() const
{
	return m_parent;
}

std::size_t DataType::ChildCount() const
{
	return 1;
}

const Entity *DataType::Child(std::size_t idx) const
{
	return idx == 0 ? &m_fields : nullptr;
}

Entity *DataType::Child(std::size_t idx)
{
	return idx == 0 ? &m_fields : nullptr;
}

std::size_t DataType::IndexOf(const Entity *child) const
{
	return child == &m_fields ? 0 : ChildCount();
}

std::string DataType::Type() const
{
	return "Class";
}

libclx::SourceLocation DataType::Location() const
{
	return m_definition;
}

std::ostream& operator<<(std::ostream& os, const DataType& c)
{
	os << "class: " << c.Name() << " (" << c.ID() << ")\n";
	for (auto&& field : c.Fields())
		os << "\t" << field << '\n';
	return os;
}

} // end of namespace
