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

#include "ModelBase.hh"
#include "codebase/CodeBase.hh"

#include <vector>

/**
 * \brief Namespace for project layer.
 *
 * This layer contains the classes that abstract elements of a project, e.g.
 * managing compiler options and code base etc. It is responsible
 * to manage all user input data that cannot be deduced from the code base.
 * These data includes the class diagram data (e.g. how many diagrams, what
 * classes does these diagrams contains).
 */
namespace project {

class Project
{
public:
	Project() = default;
	
	void SetProjectDir(const std::string& dir);
	const std::string& ProjectDir() const;
	void AddSource(const std::string& source_file);
		
	void Save(const std::string& filename) const;
	void Open(const std::string& filename, ModelFactory& factory);
	void Reload();
	
	template <typename InputIt>
	void SetCompileOptions(InputIt first, InputIt last)
	{
		m_compile_options.assign(first, last);
	}
	void SetCompileOptions(std::initializer_list<std::string> opts);
	const std::vector<std::string>& CompileOptions() const;
	
	codebase::CodeBase& CodeBase();
	
	ModelBase* Add(Model&& model);
	
	ModelBase* At(std::size_t idx);
	const ModelBase* At(std::size_t idx) const;
	std::size_t Count() const;
	void Erase(ModelBase *model);
	
private:
	std::string                 m_project_dir{"."};
	std::vector<std::string>    m_compile_options{
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", "."
	};
	codebase::CodeBase      m_code_base;
	std::vector<Model>      m_models;
};
	
} // end of namespace
