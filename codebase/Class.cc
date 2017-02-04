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
#include "EditAction.hh"

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

void Class::Visit(EditAction& data, libclx::Cursor self) const
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
			break;
		}
	});
}

void Class::Merge(EditAction&& data)
{
	data.ForEach([this](auto type, auto& entity, auto& location)
	{
		switch (type)
		{
		case EditAction::Action::addEntity:
			m_data.m_fields.push_back(std::move(dynamic_cast<Variable&>(*entity)));
			break;
			
		case EditAction::Action::setDefinition:
			m_data.m_definition = std::move(location);
			break;
		}
	});
}

boost::iterator_range<Class::field_iterator> Class::Fields() const
{
	return {m_data.m_fields.begin(), m_data.m_fields.end()};
}

const Entity* Class::Parent() const
{
	return m_parent;
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
	return dynamic_cast<const Variable*>(child) - &m_data.m_fields[0];
}

std::ostream& operator<<(std::ostream& os, const Class& c)
{
	os << "class: " << c.Name() << " (" << c.USR() << ")\n";
	for (auto&& field : c.Fields())
		os << "\t" << field << '\n';
	return os;
}

} // end of namespace
