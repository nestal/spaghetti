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

#include <boost/filesystem/path.hpp>

#include <vector>

/**
 * \brief Namespace for project layer.
 *
 * This layer contains the classes that abstract elements of a project, e.g.
 * managing compiler options and code base etc. It is responsible
 * to manage all user input data that cannot be deduced from the code base.
 * These data includes the class diagram data (e.g. how many diagrams, what
 * classes does these diagrams contains).
 *
 * All boost "path" objects in data members are absolute paths. They will
 * be converted to relative paths when saving the project to disk.
 */
namespace project {

class Project
{
public:
	Project() = default;
	
	bool SetProjectDir(const boost::filesystem::path& dir);
	const boost::filesystem::path& ProjectDir() const;
	void AddSource(const std::string& source_file);
		
	void Save(const boost::filesystem::path& path) const;
	void Open(const boost::filesystem::path& path, ModelFactory& factory);

	template <typename Func>
	void Reload(Func callback)
	{
		m_code_base.ReparseAll([this, &callback](auto map, auto root)
		{
			for (auto&& model : m_models)
				model->UpdateCodeBase(map);
			
			callback(map, root);
		});
	}
	
	template <typename InputIt>
	bool SetCompileOptions(InputIt first, InputIt last)
	{
		std::vector<std::string> opt{first, last};
		if (opt != m_compile_options)
		{
			m_compile_options = std::move(opt);
			return true;
		}
		return false;
	}
	const std::vector<std::string>& CompileOptions() const;
	
	codebase::CodeBase& CodeBase();
	
	ModelBase* Add(Model&& model);
	
	ModelBase* At(std::size_t idx);
	const ModelBase* At(std::size_t idx) const;
	std::size_t Count() const;
	void Erase(ModelBase *model);
	
private:
	boost::filesystem::path     m_project_dir{"."};
	std::vector<std::string>    m_compile_options{
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", "."
	};
	codebase::CodeBase      m_code_base;
	std::vector<Model>      m_models;
};
	
} // end of namespace
