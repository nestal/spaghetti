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

namespace cb {

Class::Class(clx::Cursor cursor) :
	m_data{cursor}
{
	
}

Class::Data::Data(clx::Cursor cursor) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()}
{
	if (cursor.IsDefinition())
	{
		std::cout << "found definition: " << cursor.Location() << std::endl;
		m_definition = cursor.Location();
	}
}

const std::string& Class::Name() const
{
	return m_data.m_name;
}

const std::string& Class::USR() const
{
	return m_data.m_usr;
}

void Class::VisitChild(Data& data, clx::Cursor child) const
{
	std::cout << Name() << " member: " << child.Spelling() << " " << child.Kind() << "\n";

	switch (child.Kind())
	{
	case CXCursor_FieldDecl:
	{
		std::cout << '\t' << child.Spelling() << ": " << child.Type().Spelling() << " " << child.USR() << "\n";
		data.m_field_usr.push_back(child.USR());
		break;
	}
	
	}
}

void Class::Merge(Class::Data&& data)
{
	m_data.m_name = std::move(data.m_name);
	m_data.m_field_usr = std::move(data.m_field_usr);
	
	if (data.m_definition)
		m_data.m_definition = std::move(data.m_definition);
}
	
} // end of namespace
