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

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

#include <QtCore/QFile>

#include <boost/filesystem.hpp>

namespace project {

using namespace boost::filesystem;

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

void Project::Save(const std::string& filename) const
{
	QJsonArray cflags;
	for (auto&& cflag : m_compile_options)
		cflags.append(QString::fromStdString(cflag));
	
	QJsonObject root;
	root["cflags"] = cflags;
	
	auto dest = path{filename}.parent_path();
	QJsonArray tus;

	for (auto&& tu : m_code_base.TranslationUnits())
		tus.append(QString::fromStdString(
			relative(tu.Spelling(), dest).string()
		));
	root["translation_units"] = tus;
	
	QJsonDocument json{root};
	
	QFile out{QString::fromStdString(filename)};
	if (out.open(QIODevice::WriteOnly))
		out.write(json.toJson());
}

void Project::Open(const std::string& filename)
{
	QFile in{QString::fromStdString(filename)};
	if (in.open(QIODevice::ReadOnly))
	{
		auto json = QJsonDocument::fromJson(in.readAll());
		
		m_compile_options.clear();
		for (auto&& cflag : json.object()["cflags"].toArray())
			m_compile_options.push_back(cflag.toString().toStdString());
		
		for (auto&& tu : json.object()["translation_units"].toArray())
			m_code_base.Parse(tu.toString().toStdString(), m_compile_options);
	}
}

} // end of namespace
