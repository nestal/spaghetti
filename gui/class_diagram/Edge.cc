/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/13/17.
//

#include "Edge.hh"

#include <QtGui/QPainter>

#include <cassert>
#include <cmath>

namespace gui {
namespace class_diagram {

Edge::Edge(const BaseItem *from, const BaseItem *to) :
	m_from{from}, m_to{to}
{
	assert(m_from);
	assert(m_to);
	
	UpdatePosition();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if (m_from->collidesWithItem(m_to))
		return;
	
	QLineF dia{
		mapFromItem(m_from, QPointF{}),
		mapFromItem(m_to,   QPointF{})
	};
	
	QPointF from_pt, to_pt;
		
	auto from_p = mapFromItem(m_from, m_from->boundingRect());
	auto to_p   = mapFromItem(m_to,   m_to->boundingRect());
	
	for (int i = 0 ; i < from_p.size() ; ++i)
	{
		QLineF line{from_p.at(i), from_p.at((i+1) % from_p.size())};
		if (dia.intersect(line, &from_pt) == QLineF::BoundedIntersection)
			break;
	}
	for (int i = 0 ; i < to_p.size() ; ++i)
	{
		QLineF line{to_p.at(i), to_p.at((i+1) % to_p.size())};
		if (dia.intersect(line, &to_pt) == QLineF::BoundedIntersection)
			break;
	}
	
	painter->drawLine(from_pt, to_pt);
	
	// build transform matrix for drawing at the point
	QTransform transform;
	
	auto relation = m_from->RelationOf(m_to);
	if (relation == ItemRelation::base_class_of)
		transform.translate(from_pt.x(), from_pt.y());
	else if (relation == ItemRelation::derived_class_of)
		transform.translate(to_pt.x(), to_pt.y());
	transform.rotateRadians(std::atan(dia.dy()/dia.dx()));
	painter->setTransform(transform);
//	painter->drawText(0,0, "Arrow");
	painter->drawPolygon(QPolygonF{} << QPointF{} << QPointF{10,10} << QPointF{-10, 10}, Qt::FillRule::WindingFill);
}

QRectF Edge::boundingRect() const
{
	return m_bounding;
}

void Edge::UpdatePosition()
{
	prepareGeometryChange();
	m_bounding = QRectF{
		mapFromItem(m_from, QPointF{}),
		mapFromItem(m_to,   QPointF{}),
	}.normalized();
}

ItemRelation Edge::RelationOf(const BaseItem *) const
{
	return ItemRelation::no_relation;
}

class_diagram::ItemType Edge::ItemType() const
{
	return ItemType::edge;
}
	
}} // end of namespace
