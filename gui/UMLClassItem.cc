/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "UMLClassItem.hh"

#include <QFont>
#include <QGraphicsTextItem>
#include <QPainter>

namespace gui {

UMLClassItem::UMLClassItem(const codebase::Class& class_, QGraphicsItem *parent) :
	QGraphicsItem{parent},
	m_class{class_},
	m_name{new QGraphicsTextItem{QString::fromStdString(m_class.Name()), this}}
{
	// use a bold font for class names
	auto font = m_name->font();
	font.setBold(true);
	m_name->setFont(font);
	
	double ypos = m_name->boundingRect().height();
	for (auto& field : m_class.Fields())
	{
		auto field_item = new QGraphicsTextItem{QString::fromStdString(field.Name()), this};
		field_item->moveBy(0, ypos);
		ypos += field_item->boundingRect().height();
	}
	
	// initialize geometry
	prepareGeometryChange();
	m_bounding = childrenBoundingRect();
	
	// flags
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF UMLClassItem::boundingRect() const
{
	return m_bounding;
}

void UMLClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	// bounding rectangle
	painter->drawRect(m_bounding);
	
	// line between class name and fields
	painter->drawLine(
		QPointF{0,                  m_name->boundingRect().height()},
		QPointF{m_bounding.width(), m_name->boundingRect().height()}
	);
}
	
} // end of namespace
