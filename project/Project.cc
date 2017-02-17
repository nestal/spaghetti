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
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <iostream>

using namespace boost::filesystem;

namespace project {

Project::Project(const std::string& dir) :
	m_dir{dir}
{
}

void Project::SetCompileOptions(std::initializer_list<std::string> opts)
{
	m_compile_options = std::move(opts);
}

codebase::CodeBase& Project::CodeBase()
{
	return m_code_base;
}

void Project::AddSource(const std::string& source_file)
{
	m_code_base.Parse(source_file, m_compile_options);
}

const std::string& Project::Dir() const
{
	return m_dir;
}

void Project::Save(const std::string& filename) const
{
	std::ofstream str{filename};
	boost::archive::text_oarchive oa{str};
	oa << *this;
}

void Project::Open(const std::string& filename)
{
	std::ifstream str{filename};
	boost::archive::text_iarchive ia{str};
	ia >> *this;
}

std::string Project::RelativePath(const std::string& path) const
{
	boost::filesystem::path p{path}, self{m_dir};
	return p.lexically_relative(self).string();
}

} // end of namespace
