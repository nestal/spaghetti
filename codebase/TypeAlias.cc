/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

#include "TypeAlias.hh"

#include "EntityType.hh"

#include "libclx/Cursor.hh"

namespace codebase {


TypeAlias::TypeAlias(const libclx::Cursor& cursor, const EntityVec *parent) :
	LeafEntity{cursor.Spelling(), cursor.USR(), parent},
	m_location{cursor.Location()},
	m_dest{cursor.TypedefUnderlying(), TypeRef{cursor}.TemplateID()}
{
}

const TypeRef& TypeAlias::Dest() const
{
	return m_dest;
}

EntityType TypeAlias::Type() const
{
	return EntityType::type_alias;
}

std::string TypeAlias::DisplayType() const
{
	return "typedef";
}

libclx::SourceLocation TypeAlias::Location() const
{
	return m_location;
}

std::string TypeAlias::UML() const
{
	return Name();
}

} // end of namespace
