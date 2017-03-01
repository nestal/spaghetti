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
	auto from = m_from->GraphicsItem();
	auto to   = m_to->GraphicsItem();
	
	if (from->collidesWithItem(to))
		return;
	
	QLineF dia{
		mapFromItem(from, QPointF{from->boundingRect().center()}),
		mapFromItem(to,   QPointF{to->boundingRect().center()})
	};
	
	QPointF from_pt, to_pt;
		
	auto from_p = mapFromItem(from, from->boundingRect());
	auto to_p   = mapFromItem(to,   to->boundingRect());
	
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
	DrawEndings(painter, to_draw);
}

QRectF Edge::boundingRect() const
{
	return m_bounding;
}

void Edge::UpdatePosition()
{
	auto from = m_from->GraphicsItem();
	auto to   = m_to->GraphicsItem();
	
	setPos(QRectF{from->boundingRect().center(), to->boundingRect().center()}.normalized().center());
	
	prepareGeometryChange();
	m_bounding = QRectF{
		mapFromItem(from, QPointF{}),
		mapFromItem(to,   QPointF{}),
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

void Edge::DrawEndings(QPainter *painter, const QLineF& dia) const
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
	DrawToEnding(painter, relation);
	
	painter->setTransform(tail);
	DrawFromEnding(painter, relation);
}

bool Edge::IsChanged() const
{
	return false;
}

void Edge::DrawToEnding(QPainter *painter, ItemRelation relation) const
{
	painter->setBrush(QBrush{Qt::GlobalColor::white});
	
	switch (relation)
	{
	case ItemRelation::base_class_of:           DrawInheritanceTrigangle(painter);  break;
	case ItemRelation::use_as_member:           DrawArrowHead(painter);             break;
	case ItemRelation::used_by_as_member:       DrawAggregationDiamond(painter);    break;
	default: break;
	}
}

void Edge::DrawFromEnding(QPainter *painter, ItemRelation relation) const
{
	painter->setBrush(QBrush{Qt::GlobalColor::white});
	
	switch (relation)
	{
	case ItemRelation::derived_class_of:        DrawInheritanceTrigangle(painter);  break;
	case ItemRelation::used_by_as_member:       DrawArrowHead(painter);             break;
	case ItemRelation::use_as_member:           DrawAggregationDiamond(painter);    break;
	default: break;
	}
}

void Edge::DrawInheritanceTrigangle(QPainter *painter) const
{
	assert(painter);
	
	painter->drawPolygon(
		QPolygonF{} << QPointF{} << QPointF{arrow_width,arrow_width} << QPointF{-arrow_width, arrow_width},
		Qt::FillRule::WindingFill
	);
}

void Edge::Update(const codebase::EntityMap*)
{
}

const BaseItem *Edge::Other(const BaseItem *one) const
{
	return m_from == one ? m_to : (
		m_to == one ? m_from : nullptr
	);
}

void Edge::DrawArrowHead(QPainter *painter) const
{
	assert(painter);
	painter->drawPolyline(
		QPolygonF{} << QPointF{arrow_width/2,arrow_width} << QPointF{} << QPointF{-arrow_width/2, arrow_width}
	);
}

void Edge::DrawAggregationDiamond(QPainter *painter) const
{
	assert(painter);
	painter->drawPolygon(
		QPolygonF{}
			<< QPointF{}
			<< QPointF{arrow_width/2,arrow_width}
			<< QPointF{0, arrow_width*2}
			<< QPointF{-arrow_width/2,arrow_width},
		Qt::FillRule::WindingFill
	);
}

QGraphicsItem *Edge::GraphicsItem()
{
	return this;
}

const QGraphicsItem *Edge::GraphicsItem() const
{
	return this;
}
	
}} // end of namespace
