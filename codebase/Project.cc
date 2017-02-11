/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/9/17.
//

#include "Project.hh"

#include <boost/filesystem.hpp>

#include <iostream>

using namespace boost::filesystem;

namespace codebase {

void Project::Open(const std::string& dir, const std::regex& filter)
{
	m_project_dir     = dir;
	m_compile_options = {
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", dir,
		"-DSRC_DIR=" + dir
	};
	
	recursive_directory_iterator it{m_project_dir};
	
	for (auto&& file : it)
	{
		auto p = file.path().string();
		if (std::regex_match(p, filter))
			m_code_base.Parse(p, m_compile_options);
	}
}

void Project::AddCompileOptions(std::initializer_list<std::string> opts)
{
	m_compile_options.insert(m_compile_options.end(), opts.begin(), opts.end());
}
	
} // end of namespace
