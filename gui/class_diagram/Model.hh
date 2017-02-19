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

#include "project/ModelBase.hh"
#include <QObject>

#include <memory>
#include <codebase/Entity.hh>

class QGraphicsScene;
class QPointF;

namespace codebase {
class DataType;
class EntityMap;
}

namespace project {
class ClassDiagram;
}

namespace gui {
namespace class_diagram {

class ClassItem;

/**
 * \brief Serializable data for class diagrams.
 */
class Model : public project::ModelBase, public QObject
{
public:
	Model(const codebase::EntityMap *codebase, const QString& name, QObject *parent);
	Model(const Model&) = delete;
	Model(Model&&) = default;
	~Model();
	
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&&) = default;
	
	QGraphicsScene* Scene();
	
	// ModelBase overrides
	void AddEntity(const std::string& id, const QPointF& pos) override;
	void SetRect(const QRectF& rect) override;
	bool CanRename() const override;
	std::string Name() const override;
	void SetName(const QString& name) override;
	void Clear();

	void Load(const QJsonObject& obj) override;
	QJsonObject Save() const override;
	
	void DeleteSelectedItem();

	ModelType Type() const override {return ModelType::class_diagram;}
	
	bool IsChanged() const override;
	
private:
	void DetectEdges(ClassItem *item);
	void AddLine(ClassItem *from, ClassItem *to);
	
private:
	QString                         m_name;
	mutable bool                    m_changed{false};
	
	std::unique_ptr<QGraphicsScene> m_scene;
	const codebase::EntityMap       *m_codebase{};
};

}} // end of namespace
