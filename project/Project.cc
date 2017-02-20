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
	QJsonObject root;
	root.insert("version", VERSION);
	
	QJsonArray cflags;
	for (auto&& cflag : m_compile_options)
		cflags.append(QString::fromStdString(cflag));
	
	root.insert("cflags", cflags);
	
	auto base = path{filename}.parent_path();
	QJsonArray tus;

	for (auto&& tu : m_code_base.TranslationUnits())
		tus.append(QString::fromStdString(
			relative(tu.Spelling(), base).string()
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
	
	QFile out{QString::fromStdString(filename)};
	if (out.open(QIODevice::WriteOnly))
		out.write(json.toJson());
}

void Project::Open(const std::string& filename, ModelFactory& factory)
{
	QFile in{QString::fromStdString(filename)};
	if (in.open(QIODevice::ReadOnly))
	{
		auto json = QJsonDocument::fromJson(in.readAll()).object();
		
		// TODO: provide backward compatibility in later versions
		if (json["version"].toString() != VERSION)
			throw std::runtime_error("version mismatch!");
		
		m_compile_options.clear();
		for (auto&& cflag : json["cflags"].toArray())
			m_compile_options.push_back(cflag.toString().toStdString());
		
		for (auto&& tu : json["translation_units"].toArray())
			m_code_base.Parse(tu.toString().toStdString(), m_compile_options);
		
		m_models.clear();
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
	
} // end of namespace
