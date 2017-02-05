/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "EditAction.hh"
#include "Entity.hh"

namespace codebase {

EditAction::~EditAction() = default;

void EditAction::SetDefinition(const libclx::SourceLocation& loc)
{
	m_ent.emplace_back(Action::setDefinition);
	m_ent.back().location = loc;
}

} // end of namespace
