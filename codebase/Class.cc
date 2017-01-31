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

#include <ostream>

namespace cb {

Class::Class(clx::Cursor cursor) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_data{cursor}
{
}

Class::Data::Data(clx::Cursor cursor)
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

void Class::Visit(Data& data, clx::Cursor self) const
{
	assert(self.Kind() == CXCursor_StructDecl || self.Kind() == CXCursor_ClassDecl);
	assert(!m_name.empty() && m_name == self.Spelling());
	assert(!m_usr.empty() && m_usr == self.USR());
	
	self.Visit([&data, this](clx::Cursor child, clx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_FieldDecl:
			data.m_fields.emplace_back(child);
			break;
			
		default:
//			std::cout << Name() << " member: " << child.Spelling() << " " << child.Kind() << "\n";
			break;
		}
	});
}

void Class::Merge(Class::Data&& data)
{
	m_data.m_fields = std::move(data.m_fields);
	
	if (data.m_definition)
		m_data.m_definition = std::move(data.m_definition);
}

boost::iterator_range<Class::field_iterator> Class::Fields() const
{
	return {m_data.m_fields.begin(), m_data.m_fields.end()};
}

Class::Field::Field(clx::Cursor field) :
	m_name{field.Spelling()},
	m_usr{field.USR()},
	m_type{field.Type()}
{
}

const std::string& Class::Field::Name() const
{
	return m_name;
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
