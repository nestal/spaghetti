/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#pragma once

#include <QtCore/QObject>
#include <QGraphicsObject>

#include "BaseItem.hh"

#include <memory>

class QGraphicsSimpleTextItem;
class QSizeF;
class QStaticText;
class QFontMetricsF;
class QJsonObject;

class SizeGripItem;

namespace codebase {
class DataType;
class Entity;
class EntityMap;
}

namespace gui {
namespace classgf {

class Edge;
struct ItemRenderingOptions;
class ClassLayout;

class ClassItem : public QGraphicsObject, public BaseItem
{
	Q_OBJECT
	
public:
	ClassItem(const std::string& id, const codebase::EntityMap *map, const QPointF& pos, const QSizeF& size = {225.0, 175.0});
	ClassItem(const QJsonObject& json, const codebase::EntityMap *map);
	~ClassItem();
	
	QGraphicsItem* GraphicsItem() override;
	const QGraphicsItem* GraphicsItem() const override;
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	const std::string& ID() const;
	const codebase::DataType& DataType() const;

	static constexpr int Type = static_cast<int>(ItemType::class_item);
	int type() const override;
	
	QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
	
	ItemRelation RelationOf(const BaseItem *other) const override;
	classgf::ItemType ItemType() const override;
	void Update(const codebase::EntityMap *map) override;
	
	bool IsChanged() const override;
	void MarkUnchanged();

	void Resize(const QRectF& rect);
	
	QString Tooltip(const ItemRenderingOptions& setting, qreal zoom_factor, const QPointF& pos) const;
	QJsonObject Save() const;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	
signals:
	void OnJustChanged(ClassItem *self);
	
private:
	void OnPositionChanged();
	void OnSelectedChange(bool selected);
	void Normalize();
	void DrawBox(QPainter *painter, const ItemRenderingOptions& setting);
	
	template <typename Member>
	void DrawMember(QPainter *painter, const Member& member, const ClassLayout& layout, std::size_t index);
	
	template <typename DrawFunc>
	void DrawUnScaled(QPainter *painter, const QPointF& pos, DrawFunc func);
	
	QString NameWithNamespace() const;
	
private:
	class Resizer;
	
	const codebase::DataType *m_class{};
	std::string        m_class_id;
	QRectF             m_bounding;

	mutable bool       m_changed{false};
	
	std::unique_ptr<SizeGripItem> m_grip;
	
	enum class MouseActionWhenRelease {select, deselect, grip, none} ;
	MouseActionWhenRelease m_release_action{MouseActionWhenRelease::none};
};
	
}} // end of namespace
