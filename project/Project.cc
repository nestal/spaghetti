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
#include "ModelBase.hh"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

#include <QtCore/QFile>

#include <boost/filesystem.hpp>
#include <iostream>

namespace project {

namespace fs = boost::filesystem;

codebase::CodeBase& Project::CodeBase()
{
	return m_code_base;
}

void Project::AddSource(const std::string& source_file)
{
	m_code_base.Parse(source_file, m_compile_options);
}

void Project::Save(const fs::path& path) const
{
	// all paths stored in disk file is relative to the parent directory
	// of the disk file
	assert(path.is_absolute());
	
	QJsonObject root;
	root.insert("version", VERSION);
	root.insert("project_dir", QString::fromStdString(
		relative(m_project_dir, path.parent_path()).string()
	));
	
	QJsonArray cflags;
	for (auto&& cflag : m_compile_options)
		cflags.append(QString::fromStdString(cflag));
	
	root.insert("cflags", cflags);
	
	QJsonArray tus;

	for (auto&& tu : m_code_base.TranslationUnits())
		tus.append(QString::fromStdString(
			relative(tu.Spelling(), path.parent_path()).string()
		));
	root.insert("translation_units", tus);
	
	// serialize all models
	QJsonArray models;
	for (auto&& model : m_models)
	{
		auto obj = model->Save();
		obj.insert("type", QString::fromStdString(to_string(model->Type())));
		obj.insert("name", QString::fromStdString(model->Name()));
		models.append(obj);
	}
	root.insert("models", models);
	
	QJsonDocument json{root};
	
	QFile out{QString::fromStdString(path.string())};
	if (out.open(QIODevice::WriteOnly))
		out.write(json.toJson());
	else
		throw std::runtime_error("can't open " + path.string());
}

void Project::Open(const fs::path& path, ModelFactory& factory)
{
	// all paths stored in disk file is relative to the parent directory
	assert(path.is_absolute());
	
	QFile in{QString::fromStdString(path.string())};
	if (in.open(QIODevice::ReadOnly))
	{
		auto json = QJsonDocument::fromJson(in.readAll()).object();
		
		auto base = path.parent_path();
		
		auto version_ok = false;
		auto version = json["version"].toString().toDouble(&version_ok);
		auto my_version = std::stod(VERSION);
		if (version > my_version)
			throw std::runtime_error("version mismatch!");
		
		// project path is relative to the parent path of the file being loaded
		auto project_path = (base/json["project_dir"].toString().toStdString()).lexically_normal();
		
		// set current path to the project path stored in the file
		// otherwise the relative paths specified by -I compile option will not work
		current_path(project_path);
		m_project_dir = project_path.string();
		
		m_compile_options.clear();
		for (auto&& cflag : json["cflags"].toArray())
			m_compile_options.push_back(cflag.toString().toStdString());

		// the models may refer to the entities in the codebase
		// clear it before reloading the codebase
		m_models.clear();
		
		for (auto&& tu : json["translation_units"].toArray())
			m_code_base.Parse((base/tu.toString().toStdString()).string(), m_compile_options);
		
		for (auto&& model_jval : json["models"].toArray())
		{
			// find the type of the model
			auto model_jobj = model_jval.toObject();
			auto type = ModelTypeFromString(model_jobj["type"].toString().toStdString());
			
			// create the model and initialize it with the json values
			auto model = factory.Create(type, model_jobj["name"].toString().toStdString(), *this);
			model->Load(model_jobj);
			
			// add to project
			Add(std::move(model));
		}
	}
}

ModelBase* Project::Add(Model&& model)
{
	m_models.push_back(std::move(model));
	return m_models.back().get();
}

ModelBase *Project::At(std::size_t idx)
{
	return m_models.at(idx).get();
}

const ModelBase *Project::At(std::size_t idx) const
{
	return m_models.at(idx).get();
}

std::size_t Project::Count() const
{
	return m_models.size();
}

void Project::Erase(ModelBase *model)
{
	auto it = std::find_if(m_models.begin(), m_models.end(), [model](auto& p){return p.get() == model;});
	if (it != m_models.end())
		m_models.erase(it);
}

const std::vector<std::string>& Project::CompileOptions() const
{
	return m_compile_options;
}

void Project::SetProjectDir(const std::string& dir)
{
	m_project_dir = dir;
}

const std::string& Project::ProjectDir() const
{
	return m_project_dir;
}

} // end of namespace
