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

#include "ClassItem.hh"
#include "codebase/DataType.hh"

#include <QtGui/QPainter>

#include <cassert>
#include <iostream>

namespace gui {
namespace class_diagram {

Edge::Edge(const ClassItem *from, const ClassItem *to) :
	m_from{from}, m_to{to}
{
	assert(m_from);
	assert(m_to);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (m_from->collidesWithItem(m_to))
		return;
	
	QLineF dia{
		mapFromItem(m_from, m_from->boundingRect().center()),
		mapFromItem(m_to,   m_to->boundingRect().center())
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
	return QRectF{
		mapFromItem(m_from, m_from->boundingRect().center()),
		mapFromItem(m_to, m_to->boundingRect().center()),
	}.normalized();
}
	
}} // end of namespace
