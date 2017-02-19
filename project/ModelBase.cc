/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/18/17.
//

#include "ModelBase.hh"

#include <algorithm>

namespace project {

static const std::pair<std::string, ModelType> map[] = {
	{"class_diagram", ModelType::class_diagram},
	{"source_view", ModelType::source_view}
};

ModelType ModelTypeFromString(const std::string& str)
{
	auto it = std::find_if(std::begin(map), std::end(map), [&str](auto& e) {return e.first == str;});
	return it != std::end(map) ? it->second : ModelType::none;
}

std::string to_string(ModelType type)
{
	auto it = std::find_if(std::begin(map), std::end(map), [type](auto& e) {return e.second == type;});
	return it != std::end(map) ? it->first : "";
}

} // end of namespace
