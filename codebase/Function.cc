/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "Function.hh"

#include <iostream>

namespace codebase {

Function::Function(libclx::Cursor first_seen, const Entity *parent) :
	LeafEntity{first_seen.Spelling(), first_seen.USR(), parent},
	m_definition{first_seen.Location()}
{
}

libclx::SourceLocation Function::Location() const
{
	return m_definition;
}

std::string Function::Type() const
{
	return "function";
}

void Function::Visit(libclx::Cursor self)
{
	assert(Parent());
	m_definition = self.Location();
}
	
} // end of namespace
