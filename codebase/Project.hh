/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/9/17.
//

#pragma once

#include "CodeBase.hh"

#include <regex>

namespace codebase {

class Project
{
public:
	Project() = default;
	
	void Open(const std::string& dir, const std::regex& filter);
	
	void AddCompileOptions(std::initializer_list<std::string> opts);
	
private:
	std::vector<std::string>    m_compile_options;
	std::string                 m_project_dir;
	
	CodeBase m_code_base;
};
	
} // end of namespace
