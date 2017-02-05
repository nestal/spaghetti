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

#include "libclx/Index.hh"
#include "libclx/SourceRange.hh"

namespace codebase {

class Variable : public LeafEntity
{
public:
	Variable(libclx::Cursor field, const std::string& parent);
	
	const std::string& Name() const override;
	const std::string& Parent() const override;
	const std::string& ID() const override;
	std::string Type() const override;
	libclx::SourceLocation Location() const override;
	
	friend std::ostream& operator<<(std::ostream& os, const Variable& c);

private:
	std::string m_name;
	std::string m_usr;
	std::string m_parent;
	
	libclx::SourceLocation m_location;
	libclx::Type   m_type;
};
	
} // end of namespace
