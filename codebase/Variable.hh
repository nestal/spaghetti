/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#pragma once

#include "Entity.hh"

#include "libclx/Type.hh"
#include "libclx/SourceRange.hh"

namespace libclx {
class Cursor;
}

namespace codebase {

class Variable : public LeafEntity
{
public:
	Variable(const libclx::Cursor& field, const EntityVec *parent);
	
	EntityType Type() const override;
	std::string DisplayType() const override;
	libclx::SourceLocation Location() const override;
	
	std::string UML() const override;
	
	friend std::ostream& operator<<(std::ostream& os, const Variable& c);

	void CrossReference(EntityMap *map) override;
	std::string TypeID() const;
	
private:
	libclx::SourceLocation m_location;
	libclx::Type           m_type;
};
	
} // end of namespace
