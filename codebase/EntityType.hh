/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#pragma once

#include <string>

namespace codebase {

enum class EntityType
{
	data_type,
	class_template,
	instantiated_type,
	function,
	namespace_,
	variable,
	type_alias,
	none
};

std::string to_string(EntityType t);

struct EntityTypeHash
{
	std::size_t operator()(EntityType t) const;
};

} // end of namespace
