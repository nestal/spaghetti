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

class SizeGripItem;

namespace codebase {
class DataType;
class Entity;
class EntityMap;
}

namespace gui {

class Edge;

class ClassItem : public QGraphicsObject, public BaseItem
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor READ GetLineColor WRITE SetLineColor DESIGNABLE true)
	
	Q_INTERFACES(QGraphicsItem)
	
public:
	ClassItem(const codebase::DataType& class_, QObject *model, const QPointF& pos, const QSizeF& size = {225.0, 175.0});
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
	gui::ItemType ItemType() const override;
	void Update(const codebase::EntityMap *map);
	
	bool IsChanged() const override;
	void MarkUnchanged();

	void Resize(const QRectF& rect);
	
	QColor GetLineColor() const;
	void SetLineColor(QColor c);
	
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	
signals:
	void OnJustChanged(ClassItem *self);
	
private:
	void ComputeSize(const QRectF& content, const QFontMetrics& name_font, const QFontMetrics& field_font);
	void OnPositionChanged();
	
private:
	class Resizer;
	
	const codebase::DataType *m_class{};
	std::string        m_class_id;
	QRectF             m_bounding;

	std::size_t        m_show_function{0}, m_show_field{0};
	mutable bool       m_changed{false};
	
	std::unique_ptr<SizeGripItem> m_grip;
	QColor m_line_color{Qt::GlobalColor::magenta};
	
	static const qreal m_margin;
};
	
} // end of namespace
