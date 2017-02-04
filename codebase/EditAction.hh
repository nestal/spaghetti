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

namespace codebase {

class EditAction
{
public:
	enum class Type {add, remove, modify};
	
private:
	Type    m_type;
};
	
} // end of namespace
