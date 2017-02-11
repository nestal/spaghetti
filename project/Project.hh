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

#include "codebase/CodeBase.hh"

#include <regex>

/**
 * \brief Namespace for project layer.
 *
 * This layer contains the classes that abstract elements of a project, e.g.
 * loading projects from file, compiling the project etc. It is responsible
 * to manage all user input data that cannot be deduced from the code base.
 * These data includes the class diagram data (e.g. how many diagrams, what
 * classes does these diagrams contains).
 */
namespace project {

class Project
{
public:
	Project(const std::string& dir = ".");
	
	void Open(const std::string& dir, const std::regex& filter);
	void AddSource(const std::string& source_file);
	
	void Save(const std::string& filename) const;
	void Load(const std::string& filename);
	
	void AddCompileOptions(std::initializer_list<std::string> opts);
	
	codebase::CodeBase& CodeBase();
	
	const std::string& Dir() const;
	
private:
	std::vector<std::string>    m_compile_options{
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", "."
	};
	std::string             m_dir;
	codebase::CodeBase      m_code_base;
};
	
} // end of namespace
