/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "ClassItem.hh"
#include "Edge.hh"

#include "codebase/DataType.hh"

#include <QtGui/QFont>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsScene>
#include <iostream>

namespace gui {
namespace class_diagram {

const qreal ClassItem::m_margin{10.0};

const qreal ClassItem::m_max_width{200.0};

ClassItem::ClassItem(const codebase::DataType& class_, QGraphicsItem *parent) :
	BaseItem{parent},
	m_class{class_},
	m_name{new QGraphicsSimpleTextItem{QString::fromStdString(m_class.Name()), this}}
{
	
	// use a bold font for class names
	auto font = m_name->font();
	font.setBold(true);
	m_name->setFont(font);
	
	auto dx = m_name->boundingRect().width();
	auto dy = m_name->boundingRect().height();
	for (auto& field : m_class.Fields())
	{
		auto field_item = new QGraphicsSimpleTextItem{
			QFontMetrics{QFont{}}.elidedText(
				QString::fromStdString(field.Name() + ":" + field.Type()),
				Qt::ElideRight,
				static_cast<int>(std::max(m_name->boundingRect().width(), m_max_width)),
				0
			),
			this
		};
		field_item->moveBy(0, dy);
		
		auto rect = field_item->boundingRect();
		dy += rect.height();
		dx = std::max(dx, rect.width());
	}
	
	// make all children center at origin
	for (auto child : childItems())
		child->moveBy(-dx/2, -dy/2);
	
	// initialize geometry
	prepareGeometryChange();
	m_bounding.setCoords(-dx/2-m_margin, -dy/2-m_margin, dx/2+m_margin, dy/2+m_margin);
	
	// flags
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

ClassItem::~ClassItem() = default;

QRectF ClassItem::boundingRect() const
{
	return m_bounding;
}

void ClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	// TODO: make it configurable
	painter->setPen(QPen{QColor{Qt::GlobalColor::magenta}});
	painter->setBrush(QBrush{QColor{Qt::GlobalColor::yellow}});
	
	// bounding rectangle
	painter->drawRect(m_bounding);
	
	// line between class name and fields
	auto ypos = m_name->y() + m_name->boundingRect().height();
	painter->drawLine(
		QPointF{m_bounding.left(), ypos},
		QPointF{m_bounding.right(), ypos}
	);
}

const std::string& ClassItem::ID() const
{
	return m_class.ID();
}

const codebase::DataType& ClassItem::DataType() const
{
	return m_class;
}

int ClassItem::type() const
{
	return Type;
}

QVariant ClassItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (change == QGraphicsItem::ItemPositionChange)
	{
		for (auto&& edge : m_edges)
			edge->UpdatePosition();
	}

	return value;
}

void ClassItem::AddEdge(Edge *edge)
{
	m_edges.push_back(edge);
}

ItemRelation ClassItem::RelationOf(const BaseItem *other) const
{
	assert(other);
	switch (other->ItemType())
	{
	case ItemType::class_item:
	{
		auto class_ = qgraphicsitem_cast<const ClassItem*>(other);
		assert(class_);
		
		if (class_->m_class.IsBaseOf(m_class))
			return ItemRelation::derived_class_of;
		
		else if (m_class.IsBaseOf(class_->m_class))
			return ItemRelation::base_class_of;
		
		// fall through
	}
	default:    return ItemRelation::no_relation;
	}
}

class_diagram::ItemType ClassItem::ItemType() const
{
	return ItemType::class_item;
}
	
}} // end of namespace
