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

namespace project {

Project::Project(const std::string& dir) :
	m_dir{dir}
{
}

void Project::Open(const std::string& dir, const std::regex& filter)
{
	m_dir     = dir;
	m_compile_options = {
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", dir,
		"-DSRC_DIR=" + dir
	};
	
	recursive_directory_iterator it{m_dir};
	
	for (auto&& file : it)
	{
		auto p = file.path().string();
		if (std::regex_match(p, filter))
			AddSource(p);
	}
}

void Project::AddCompileOptions(std::initializer_list<std::string> opts)
{
	m_compile_options.insert(m_compile_options.end(), opts.begin(), opts.end());
}

codebase::CodeBase& Project::CodeBase()
{
	return m_code_base;
}

void Project::AddSource(const std::string& file)
{
	m_code_base.Parse(file, m_compile_options);
}

const std::string& Project::Dir() const
{
	return m_dir;
}
	
} // end of namespace
