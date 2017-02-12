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

#include "ModelBase.hh"
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

class SceneModel : public ModelBase, public QObject
{
public:
	SceneModel(const codebase::EntityMap *codebase, QObject *parent);
	SceneModel(const SceneModel&) = delete;
	SceneModel(SceneModel&&) = default;
	~SceneModel();
	
	SceneModel& operator=(const SceneModel&) = delete;
	SceneModel& operator=(SceneModel&&) = default;
	
	QGraphicsScene* Scene() override;
	void SetRect(const QRectF& rect) override;
	void Clear();
	
	void AddEntity(const std::string& id, const QPointF& pos) override;
	
public:
	std::unique_ptr<QGraphicsScene> m_scene;
	const codebase::EntityMap       *m_codebase{};
};

}} // end of namespace
