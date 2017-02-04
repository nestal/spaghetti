/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "Class.hh"

#include <iostream>

namespace codebase {

Class::Class(libclx::Cursor cursor, const Entity *parent) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_parent{parent},
	m_data{cursor}
{
}

Class::Data::Data(libclx::Cursor cursor)
{
	assert(cursor.Kind() == CXCursor_StructDecl || cursor.Kind() == CXCursor_ClassDecl);
	
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}

const std::string& Class::Name() const
{
	return m_name;
}

const std::string& Class::USR() const
{
	return m_usr;
}

void Class::Visit(Data& data, libclx::Cursor self) const
{
	assert(self.Kind() == CXCursor_StructDecl || self.Kind() == CXCursor_ClassDecl);
	assert(!m_name.empty() && m_name == self.Spelling());
	assert(!m_usr.empty() && m_usr == self.USR());
	
	self.Visit([&data, this](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_FieldDecl:
			data.m_fields.emplace_back(child, this);
			break;
			
		default:
			break;
		}
	});
}

void Class::Merge(Class::Data&& data)
{
	m_data.m_fields.insert(m_data.m_fields.end(),
		make_move_iterator(data.m_fields.begin()),
		make_move_iterator(data.m_fields.end())
	);
	
	if (!m_data.m_definition && data.m_definition)
		m_data.m_definition = std::move(data.m_definition);
}

boost::iterator_range<Class::field_iterator> Class::Fields() const
{
	return {m_data.m_fields.begin(), m_data.m_fields.end()};
}

std::size_t Class::FieldCount() const
{
	return m_data.m_fields.size();
}

const Entity* Class::Parent() const
{
	return m_parent;
}

const Class::Field& Class::FieldAt(std::size_t idx) const
{
	return m_data.m_fields.at(idx);
}

std::size_t Class::ChildCount() const
{
	return m_data.m_fields.size();
}

const Entity *Class::Child(std::size_t idx) const
{
	return &m_data.m_fields.at(idx);
}

std::size_t Class::IndexOf(const Entity *child) const
{
	return dynamic_cast<const Field*>(child) - &m_data.m_fields[0];
}

Class::Field::Field(libclx::Cursor field, const Class *parent) :
	m_name{field.Spelling()},
	m_usr{field.USR()},
	m_parent{parent},
	m_type{field.Type()}
{
}

const std::string& Class::Field::Name() const
{
	return m_name;
}

const Class* Class::Field::Parent() const
{
	return m_parent;
}

const std::string& Class::Field::USR() const
{
	return m_usr;
}

std::size_t Class::Field::ChildCount() const
{
	return 0;
}

const Entity *Class::Field::Child(std::size_t) const
{
	return nullptr;
}

std::size_t Class::Field::IndexOf(const Entity*) const
{
	return 0;
}

std::ostream& operator<<(std::ostream& os, const Class::Field& c)
{
	return os << c.m_name << ": " << c.m_type;
}

std::ostream& operator<<(std::ostream& os, const Class& c)
{
	os << "class: " << c.Name() << " (" << c.USR() << ")\n";
	for (auto&& field : c.Fields())
		os << "\t" << field << '\n';
	return os;
}

} // end of namespace
