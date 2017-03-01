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
namespace class_diagram {

class Edge;

class ClassItem : public QObject, public BaseItem
{
	Q_OBJECT

public:
	ClassItem(const codebase::DataType& class_, const QPointF& pos, QObject *model);
	~ClassItem();
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	const std::string& ID() const;
	const codebase::DataType& DataType() const;

	static constexpr int Type = static_cast<int>(ItemType::class_item);
	int type() const override;
	
	QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
	
	ItemRelation RelationOf(const BaseItem *other) const override;
	class_diagram::ItemType ItemType() const override;
	void Update(const codebase::EntityMap *map);
	
	bool IsChanged() const override;
	void MarkUnchanged();

	void Resize(const QRectF& rect);
	
signals:
	void OnJustChanged(ClassItem *self);
	
private:
	void ComputeSize(const QRectF& content, const QFontMetrics& name_font, const QFontMetrics& field_font);
	
private:
	class Resizer;
	
	const codebase::DataType *m_class{};
	std::string        m_class_id;
	QRectF             m_bounding;

	std::size_t        m_show_function{0}, m_show_field{0};
	mutable bool       m_changed{false};
	
	std::unique_ptr<SizeGripItem> m_grip;
	
	static const qreal m_margin;
};
	
}} // end of namespace
