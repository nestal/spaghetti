/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "Type.hh"

#include "Cursor.hh"
#include "XStr.hh"

#include <ostream>

namespace libclx {

Type::Type(CXType type) :
	m_type{type}
{
}

std::string Type::Spelling() const
{
	return XStr{::clang_getTypeSpelling(m_type)}.Str();
}

std::ostream& operator<<(std::ostream& os, const Type& t)
{
	return os << t.Spelling() << ": " << " (" << t.Kind() << ")";
}

Cursor Type::Declaration() const
{
	return {::clang_getTypeDeclaration(m_type)};
}

std::string Type::Kind() const
{
	return XStr{::clang_getTypeKindSpelling(m_type.kind)}.Str();
}

Type Type::ClassType() const
{
	return {::clang_Type_getClassType(m_type)};
}

} // end of namespace
