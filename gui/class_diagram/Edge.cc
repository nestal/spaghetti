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

#include <cassert>

namespace gui {
namespace class_diagram {

Edge::Edge(const ClassItem *from, const ClassItem *to) :
	m_from{from}, m_to{to}
{
	assert(m_from);
	assert(m_to);
}

void Edge::UpdatePosition()
{
	
}

QLineF Edge::Calculate() const
{
	return LineFrom(QRectF{
			m_from->mapToScene(m_from->boundingRect().topLeft()),
			m_from->mapToScene(m_from->boundingRect().bottomRight())
		}, QRectF{
			m_to->mapToScene(m_to->boundingRect().topLeft()),
			m_to->mapToScene(m_to->boundingRect().bottomRight())
		});
}

QLineF Edge::LineFrom(const QRectF& from, const QRectF& to)
{
	QLineF dia{from.center(), to.center()};
	
	QPointF from_pt, to_pt;
	
	// check intersection with "from" rectangle
	if (dia.intersect(QLineF{from.topLeft(),    from.topRight()},    &from_pt) == QLineF::BoundedIntersection) goto next;
	if (dia.intersect(QLineF{from.bottomLeft(), from.bottomRight()}, &from_pt) == QLineF::BoundedIntersection) goto next;
	if (dia.intersect(QLineF{from.topLeft(),    from.bottomLeft()},  &from_pt) == QLineF::BoundedIntersection) goto next;
	if (dia.intersect(QLineF{from.topRight(),   from.bottomRight()}, &from_pt) == QLineF::BoundedIntersection) goto next;
	
	// check intersection with the "to" rectangle
	next:
	if (dia.intersect(QLineF{to.topLeft(),    to.topRight()},    &to_pt) == QLineF::BoundedIntersection) goto done;
	if (dia.intersect(QLineF{to.bottomLeft(), to.bottomRight()}, &to_pt) == QLineF::BoundedIntersection) goto done;
	if (dia.intersect(QLineF{to.topLeft(),    to.bottomLeft()},  &to_pt) == QLineF::BoundedIntersection) goto done;
	if (dia.intersect(QLineF{to.topRight(),   to.bottomRight()}, &to_pt) == QLineF::BoundedIntersection) goto done;
	
	done:
	return {from_pt, to_pt};
}

QRectF Edge::boundingRect() const
{
	auto line = Calculate();
	auto rect = QRectF{line.p1(), line.p2()}.normalized();
	rect.translate(-rect.center().x(), -rect.center().y());
	return rect;
}

void Edge::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
	
}
	
}} // end of namespace
