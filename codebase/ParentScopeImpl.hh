/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/23/17.
//

#pragma once

#include "Function.hh"
#include "Variable.hh"
#include "ClassTemplate.hh"
#include "Namespace.hh"

namespace codebase {

struct ParentScope::Impl
{
	using Cond = util::MultiContainer<
		Entity,
		Variable,
		codebase::Function,
		DataType,
		ClassTemplate,
		
		// will remove later
		Namespace
	>;
	Cond cond;
};
	
} // end of namespace
