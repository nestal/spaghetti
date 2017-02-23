/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#pragma once

#include "EntityVec.hh"

#include "libclx/SourceRange.hh"

namespace libclx {
class Cursor;
}

namespace codebase {

class Function : public EntityVec
{
public:
	Function(libclx::Cursor first_seen, const Entity *parent);
	
	void Visit(libclx::Cursor self);
	
	std::string Type() const override;
	libclx::SourceLocation Location() const override;
	
	void RemoveUnused() override;
	
private:
	libclx::SourceLocation m_definition;
};
	
} // end of namespace
