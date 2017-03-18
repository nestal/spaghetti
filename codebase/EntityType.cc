/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#include "EntityType.hh"

#include <cassert>

namespace codebase {

std::string to_string(EntityType t)
{
	static const std::string types[] =
	{
		"data_type",
		"class_template",
		"instantiated_type",
		"function",
		"namespace",
		"variable",
		"none"
	};
	
	auto it = static_cast<int>(t);
	assert(it >= static_cast<int>(EntityType::data_type));
	assert(it <= static_cast<int>(EntityType::none));
	
	return types[it];
}

std::size_t EntityTypeHash::operator()(EntityType t) const
{
	return static_cast<std::size_t>(t);
}
}