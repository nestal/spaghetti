/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

#pragma once

#include "Entity.hh"
#include "TypeRef.hh"

#include "libclx/SourceRange.hh"

namespace libclx {
class Cursor;
}

namespace codebase {

class TypeAlias : public LeafEntity
{
public:
	TypeAlias(const libclx::Cursor& cursor, const EntityVec *parent);
	
	const TypeRef& Dest() const;
	
	EntityType Type() const override;
	std::string DisplayType() const override;
	libclx::SourceLocation Location() const override;
	
	std::string UML() const override;

private:
	libclx::SourceLocation  m_location;
	TypeRef m_dest;
};
	
} // end of namespace
