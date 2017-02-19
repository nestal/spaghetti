/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#pragma once

#include <string>
#include <memory>

class QPointF;
class QRectF;
class QString;
class QJsonObject;

namespace project {

enum class ModelType {
	class_diagram,
	source_view,
	none
};

ModelType ModelTypeFromString(const std::string& str);
std::string to_string(ModelType type);

class ModelBase
{
public:
	using ModelType = project::ModelType;
	
	virtual ~ModelBase() = default;
	
	virtual void AddEntity(const std::string& id, const QPointF& pos) = 0;
	virtual void SetRect(const QRectF& rect) = 0;
	
	virtual void Load(const QJsonObject& obj) = 0;
	virtual QJsonObject Save() const = 0;
	
	virtual bool CanRename() const = 0;
	virtual std::string Name() const = 0;
	virtual void SetName(const QString& name) = 0;
	virtual ModelType Type() const = 0;
};

using Model = std::unique_ptr<ModelBase>;

class Project;

class ModelFactory
{
public:
	virtual ~ModelFactory() = default;
	
	virtual Model Create(ModelType type, const std::string& name, Project& owner) = 0;
};

} // end of namespace
