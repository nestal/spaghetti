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

#include <QtCore/QObject>
#include <QtCore/QRectF>

#include <memory>

class QGraphicsScene;
class QPointF;
class QMimeData;

namespace codebase {
class DataType;
class EntityMap;
class Entity;
}

namespace gui {
namespace classgf {

class ClassItem;
class BaseItem;

/**
 * \brief Serializable data for class diagrams.
 */
class ClassModel : public QObject, public project::ModelBase
{
Q_OBJECT

public:
	ClassModel(const codebase::EntityMap *codebase, const QString& name, QObject *parent);
	ClassModel(const ClassModel&) = delete;
	ClassModel(ClassModel&&) = default;
	~ClassModel();
	
	ClassModel& operator=(const ClassModel&) = delete;
	ClassModel& operator=(ClassModel&&) = default;
	
	QGraphicsScene *Scene();
	
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
	void DeleteItem(ClassItem *item);
	void AddParentClass(ClassItem *item, const QPointF& pos);
	
	ModelType Type() const override { return ModelType::class_diagram; }
	
	bool IsChanged() const override;
	void UpdateCodeBase(const codebase::EntityMap *codebase) override;
	
	std::unique_ptr<QMimeData> CopySelection() const;
	QImage RenderImage(const QRectF& rect = {}) const;
	QByteArray RenderSVG(const QRectF& rect = {}) const;
	
	void Paste(const QMimeData* data);
	
signals:
	void OnChanged(bool changed) const;

private:
	template <typename... Args>
	void AddItem(Args&&... args);
	void DetectEdges(ClassItem *item);
	void AddLine(ClassItem *from, ClassItem *to);
	void OnChildChanged(BaseItem *child);
	void SetChanged(bool changed) const;

private:
	QString m_name;
	mutable bool m_changed{false};
	
	std::unique_ptr<QGraphicsScene> m_scene;
	const codebase::EntityMap       *m_codebase{};
};
	
}} // end of namespace
