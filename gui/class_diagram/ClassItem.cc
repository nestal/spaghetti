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
	QGraphicsItem{parent},
	m_class{class_},
	m_name{new QGraphicsSimpleTextItem{QString::fromStdString(m_class.Name()), this}}
{
	
	// use a bold font for class names
	auto font = m_name->font();
	font.setBold(true);
	m_name->setFont(font);
	m_name->moveBy(0, 0);
	
	auto maxx = m_name->boundingRect().width();
	auto ypos = m_name->boundingRect().height();
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
		field_item->moveBy(0, ypos);
		ypos += field_item->boundingRect().height();
		maxx = std::max(maxx, field_item->boundingRect().width());
	}
	
	// make all children center at origin
	for (auto child : childItems())
		child->moveBy(-maxx/2, -ypos/2);
	
	// initialize geometry
	prepareGeometryChange();
	m_bounding = childrenBoundingRect();
	m_bounding.adjust(-m_margin, -m_margin, m_margin, m_margin);
	
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
	painter->setPen(QPen{QColor{"purple"}});
	painter->setBrush(QBrush{QColor{"yellow"}});
	
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
	
}} // end of namespace
