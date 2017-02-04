/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "DataType.hh"
#include "EditAction.hh"

#include <iostream>

namespace codebase {

DataType::DataType(libclx::Cursor cursor, const Entity *parent) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_parent{parent}
{
	assert(cursor.Kind() == CXCursor_StructDecl || cursor.Kind() == CXCursor_ClassDecl);
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}

const std::string& DataType::Name() const
{
	return m_name;
}

const std::string& DataType::USR() const
{
	return m_usr;
}

void DataType::Visit(EditAction& data, libclx::Cursor self) const
{
	assert(self.Kind() == CXCursor_StructDecl || self.Kind() == CXCursor_ClassDecl);
	assert(!m_name.empty() && m_name == self.Spelling());
	assert(!m_usr.empty() && m_usr == self.USR());
	
	if (self.IsDefinition())
		data.SetDefinition(self.Location());
	
	self.Visit([&data, this](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_FieldDecl:
			data.AddEntity(Variable{child, this});
			break;
	
		default:
		std::cout << child.Spelling() << ' ' << child.Kind() << std::endl;
			break;
		}
	});
}

void DataType::Merge(EditAction&& data)
{
	data.ForEach([this](auto type, auto& entity, auto& location)
	{
		switch (type)
		{
		case EditAction::Action::addEntity:
			m_fields.push_back(std::move(dynamic_cast<Variable&>(*entity)));
			break;
			
		case EditAction::Action::setDefinition:
			m_definition = std::move(location);
			break;
		}
	});
}

boost::iterator_range<DataType::field_iterator> DataType::Fields() const
{
	return {m_fields.begin(), m_fields.end()};
}

const Entity* DataType::Parent() const
{
	return m_parent;
}

std::size_t DataType::ChildCount() const
{
	return m_fields.size();
}

const Entity *DataType::Child(std::size_t idx) const
{
	return &m_fields.at(idx);
}

std::size_t DataType::IndexOf(const Entity *child) const
{
	return dynamic_cast<const Variable*>(child) - &m_fields[0];
}

std::string DataType::Type() const
{
	return "DataType";
}

libclx::SourceLocation DataType::DefinitionLocation() const
{
	return m_definition;
}

std::ostream& operator<<(std::ostream& os, const DataType& c)
{
	os << "class: " << c.Name() << " (" << c.USR() << ")\n";
	for (auto&& field : c.Fields())
		os << "\t" << field << '\n';
	return os;
}

} // end of namespace
