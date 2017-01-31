/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//


#pragma once

#include <string>

namespace cb {

class CppClass
{
private:
	CppClass(const std::string& name);
	
	void AddMemberFunction(const std::string& name);
	void AddDataMember(const std::string& name);
	
private:
	std::string m_name;
};
	
} // end of namespace
