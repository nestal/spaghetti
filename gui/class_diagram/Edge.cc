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

namespace gui {
namespace class_diagram {

Edge::Edge(const QGraphicsItem *from, const QGraphicsItem *to) :
	m_from{from}, m_to{to}
{
	assert(m_from);
	assert(m_to);
	
	UpdatePosition();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (m_from->collidesWithItem(m_to))
		return;
	
	QLineF dia{
		mapFromItem(m_from, 0, 0),
		mapFromItem(m_to,   0, 0)
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
}

QRectF Edge::boundingRect() const
{
	return m_bounding;
}

void Edge::UpdatePosition()
{
	prepareGeometryChange();
	m_bounding = QRectF{
		mapFromItem(m_from, 0, 0),
		mapFromItem(m_to, 0, 0),
	}.normalized();
}
	
}} // end of namespace
