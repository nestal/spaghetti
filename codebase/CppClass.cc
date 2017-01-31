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
	m_cursor{cursor}
{
}

std::string CppClass::Name() const
{
	return m_cursor.Spelling();
}

clx::Cursor CppClass::Cursor() const
{
	return m_cursor;
}

std::string CppClass::USR() const
{
	return m_cursor.USR();
}

void CppClass::Visit(clx::Cursor cursor, clx::Cursor parent)
{
	std::cout << Name() << " member: " << cursor.Spelling() << " " << cursor.Kind() << "\n";
}
	
} // end of namespace
