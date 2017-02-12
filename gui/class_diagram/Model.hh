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

#include "gui/common/ModelBase.hh"
#include <QObject>

#include <memory>
#include <codebase/Entity.hh>

class QGraphicsScene;
class QPointF;

namespace codebase {
class DataType;
class EntityMap;
}

namespace gui {
namespace class_diagram {

class Model : public common::ModelBase, public QObject
{
public:
	Model(const codebase::EntityMap *codebase, const QString& name, QObject *parent);
	Model(const Model&) = delete;
	Model(Model&&) = default;
	~Model();
	
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&&) = default;
	
	QGraphicsScene* Scene();
	
	void AddEntity(const std::string& id, const QPointF& pos) override;
	void SetRect(const QRectF& rect) override;
	bool CanRename() const override;
	QString Name() const override;
	void SetName(const QString& name) override;
	void Clear();

public:
	QString                         m_name;
	
	std::unique_ptr<QGraphicsScene> m_scene;
	const codebase::EntityMap       *m_codebase{};
};

}} // end of namespace
