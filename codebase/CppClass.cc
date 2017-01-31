/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "CppClass.hh"

#include <iostream>

namespace cb {

CppClass::CppClass(clx::Cursor cursor) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_type{cursor.Type()}
{
}

const std::string& CppClass::Name() const
{
	return m_name;
}

const std::string& CppClass::USR() const
{
	return m_usr;
}

void CppClass::Visit(clx::Cursor cursor, clx::Cursor parent)
{
	std::cout << Name() << " member: " << cursor.Spelling() << " " << cursor.Kind() << "\n";
	
	switch (cursor.Kind())
	{
	case CXCursor_FieldDecl:
	{
		std::cout << '\t' << cursor.Spelling() << ": " << cursor.Type().Spelling() << " " << cursor.USR() << "\n";
		m_field_usr.push_back(cursor.USR());
		break;
	}
	
	}
}
	
} // end of namespace
