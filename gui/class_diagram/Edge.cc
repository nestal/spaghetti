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
#include <iostream>

namespace gui {
namespace class_diagram {

const auto arrow_width = 15.0;

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
	
	QLineF to_draw{from_pt, to_pt};
	painter->drawLine(to_draw);
	DrawArrow(painter, to_draw);
}

QRectF Edge::boundingRect() const
{
	return m_bounding;
}

void Edge::UpdatePosition()
{
	setPos(QRectF{m_from->pos(), m_to->pos()}.normalized().center());
	
	prepareGeometryChange();
	m_bounding = QRectF{
		mapFromItem(m_from, QPointF{}),
		mapFromItem(m_to,   QPointF{}),
	}.normalized().adjusted(-arrow_width, -arrow_width, arrow_width, arrow_width);
}

ItemRelation Edge::RelationOf(const BaseItem *) const
{
	return ItemRelation::no_relation;
}

class_diagram::ItemType Edge::ItemType() const
{
	return ItemType::edge;
}

void Edge::DrawArrow(QPainter *painter, const QLineF& dia) const
{
	auto from_pt = dia.p1();
	auto to_pt   = dia.p2();
	
	// build transform matrix for drawing at the point
	QTransform head{painter->transform()}, tail{painter->transform()};
	head.translate(from_pt.x(), from_pt.y());
	tail.translate(to_pt.x(), to_pt.y());
	
	auto angle = -std::atan(dia.dx()/dia.dy());
	if (dia.dy() < 0)
		angle += M_PI;
	head.rotateRadians(angle);
	tail.rotateRadians(angle - M_PI);
	
	auto relation = m_from->RelationOf(m_to);
	painter->setTransform(head);
	DrawArrowHead(painter, relation);
	
	painter->setTransform(tail);
	DrawArrowTail(painter, relation);
}

bool Edge::IsChanged() const
{
	return false;
}

void Edge::DrawArrowHead(QPainter *painter, ItemRelation relation) const
{
	painter->setBrush(QBrush{Qt::GlobalColor::white});
	
	// draw a triangle
	if (relation == ItemRelation::base_class_of || relation == ItemRelation::derived_class_of)
		painter->drawPolygon(
			QPolygonF{} << QPointF{} << QPointF{arrow_width,arrow_width} << QPointF{-arrow_width, arrow_width},
			Qt::FillRule::WindingFill
		);
		
	// draw an arrow head
	else if (relation == ItemRelation::use_as_member|| relation == ItemRelation::used_by_as_member)
		painter->drawPolyline(
			QPolygonF{} << QPointF{arrow_width/2,arrow_width} << QPointF{} << QPointF{-arrow_width/2, arrow_width}
		);
}

void Edge::DrawArrowTail(QPainter *painter, ItemRelation relation) const
{
	painter->setBrush(QBrush{Qt::GlobalColor::white});
	
	// draw a diamond
	if (relation == ItemRelation::use_as_member|| relation == ItemRelation::used_by_as_member)
		painter->drawPolygon(
			QPolygonF{}
				<< QPointF{}
				<< QPointF{arrow_width/2,arrow_width}
				<< QPointF{0, arrow_width*2}
				<< QPointF{-arrow_width/2,arrow_width},
			Qt::FillRule::WindingFill
		);
}

void Edge::Update(const codebase::CodeBase&)
{
}
	
}} // end of namespace
