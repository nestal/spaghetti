/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#pragma once

#include <string>

namespace codebase {

class Entity
{
public:
	virtual ~Entity() = default;
	
	virtual const std::string& Name() const = 0;
	virtual const std::string& USR() const = 0;
	virtual const std::string& Parent() const = 0;
};
	
} // end of namespace