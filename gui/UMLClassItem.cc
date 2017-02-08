/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "UMLClassItem.hh"

#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QPainter>

namespace gui {

const qreal UMLClassItem::m_margin = 10.0;

UMLClassItem::UMLClassItem(const codebase::DataType& class_, QGraphicsItem *parent) :
	QGraphicsItem{parent},
	m_class{class_},
	m_name{new QGraphicsSimpleTextItem{QString::fromStdString(m_class.Name()), this}}
{
	
	// use a bold font for class names
	auto font = m_name->font();
	font.setBold(true);
	m_name->setFont(font);
	m_name->moveBy(m_margin, m_margin);
	
	double ypos = m_name->boundingRect().height();
	for (auto& field : m_class.Fields())
	{
		auto field_item = new QGraphicsSimpleTextItem{QString::fromStdString(field.Name()), this};
		field_item->moveBy(m_margin, ypos + m_margin);
		ypos += field_item->boundingRect().height();
	}
	
	// initialize geometry
	prepareGeometryChange();
	m_bounding = childrenBoundingRect();
	m_bounding.adjust(-m_margin, -m_margin, m_margin, m_margin);
	
	// flags
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
}

UMLClassItem::~UMLClassItem() = default;

QRectF UMLClassItem::boundingRect() const
{
	return m_bounding;
}

void UMLClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
{
	// bounding rectangle
	painter->drawRect(m_bounding);
	
	// line between class name and fields
	painter->drawLine(
		QPointF{0,                  m_name->boundingRect().height() + m_margin},
		QPointF{m_bounding.width(), m_name->boundingRect().height() + m_margin}
	);
}

const std::string& UMLClassItem::ID() const
{
	return m_class.ID();
}
	
} // end of namespace
