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
namespace classgf {

const auto arrow_width = 15.0;

Edge::Edge(BaseItem *from, BaseItem *to) :
	m_from{from}, m_to{to}
{
	assert(m_from);
	assert(m_to);
	assert(m_from != m_to);
	
	UpdatePosition();
}

QLineF Edge::Dia() const
{
	auto from = m_from->GraphicsItem();
	auto to = m_to->GraphicsItem();
	
	QLineF dia{
		mapFromItem(from, QPointF{from->boundingRect().center()}),
		mapFromItem(to,   QPointF{to->boundingRect().center()})
	};
	
	QPointF from_pt, to_pt;
	
	auto from_p = mapFromItem(from, from->boundingRect());
	auto to_p   = mapFromItem(to, to->boundingRect());
	
	for (int i = 0; i < from_p.size(); ++i)
	{
		QLineF line{from_p.at(i), from_p.at((i + 1) % from_p.size())};
		if (dia.intersect(line, &from_pt) == QLineF::BoundedIntersection)
			break;
	}
	for (int i = 0; i < to_p.size(); ++i)
	{
		QLineF line{to_p.at(i), to_p.at((i + 1) % to_p.size())};
		if (dia.intersect(line, &to_pt) == QLineF::BoundedIntersection)
			break;
	}
	return {from_pt, to_pt};
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	auto from = m_from->GraphicsItem();
	auto to = m_to->GraphicsItem();
	
	if (from->collidesWithItem(to))
		return;
	
	QPen line_pen{Qt::GlobalColor::black};
	line_pen.setCosmetic(true);
	painter->setPen(line_pen);
	
	auto to_draw = Dia();
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
	auto to = m_to->GraphicsItem();
	
	auto from_center = from->boundingRect().center();
	auto to_center = to->boundingRect().center();
	
	setPos(QRectF{from_center, to_center}.normalized().center());
	
	prepareGeometryChange();
	m_bounding = QRectF{
		mapFromItem(from, from_center),
		mapFromItem(to,   to_center),
	}.normalized().adjusted(-arrow_width, -arrow_width, arrow_width, arrow_width);
}

ItemRelation Edge::RelationOf(const BaseItem *) const
{
	return ItemRelation::no_relation;
}

classgf::ItemType Edge::ItemType() const
{
	return ItemType::edge;
}

void Edge::DrawEndings(QPainter *painter, const QLineF& dia) const
{
	auto from_pt = dia.p1();
	auto to_pt = dia.p2();
	
	// build transform matrix for drawing at the point
	QTransform head{painter->transform()}, tail{painter->transform()};
	head.translate(from_pt.x(), from_pt.y());
	tail.translate(to_pt.x(), to_pt.y());
	
	auto angle = -std::atan(dia.dx() / dia.dy());
	if (dia.dy() < 0)
		angle += M_PI;
	head.rotateRadians(angle);
	tail.rotateRadians(angle - M_PI);
	
	auto relation = m_from->RelationOf(m_to);
	painter->setTransform(head);
	DrawFromEnding(painter, relation);
	
	painter->setTransform(tail);
	DrawToEnding(painter, relation);
}

bool Edge::IsChanged() const
{
	return false;
}

void Edge::DrawFromEnding(QPainter *painter, ItemRelation relation) const
{
	painter->setBrush(QBrush{Qt::GlobalColor::white});
	
	switch (relation)
	{
	case ItemRelation::base_class_of: DrawInheritanceTrigangle(painter);
		break;
	case ItemRelation::use_as_member: DrawAggregationDiamond(painter);
		break;
	case ItemRelation::used_by_as_member: DrawArrowHead(painter);
		break;
	default: break;
	}
}

void Edge::DrawToEnding(QPainter *painter, ItemRelation relation) const
{
	painter->setBrush(QBrush{Qt::GlobalColor::white});
	
	switch (relation)
	{
	// m_from is derived class of m_to
	case ItemRelation::derived_class_of: DrawInheritanceTrigangle(painter);
		break;
		
	// m_from is used as member by m_to
	case ItemRelation::used_by_as_member: DrawAggregationDiamond(painter);
		break;
		
	// m_from used m_to as member
	case ItemRelation::use_as_member: DrawArrowHead(painter);
		break;
	default: break;
	}
}

void Edge::DrawInheritanceTrigangle(QPainter *painter) const
{
	assert(painter);
	
	painter->drawPolygon(
		QPolygonF{} << QPointF{} << QPointF{arrow_width, arrow_width} << QPointF{-arrow_width, arrow_width},
		Qt::FillRule::WindingFill
	);
}

void Edge::Update(const codebase::EntityMap *)
{
}

const BaseItem *Edge::Other(const BaseItem *one) const
{
	return m_from == one ? m_to : (
		m_to == one ? m_from : nullptr
	);
}

BaseItem *Edge::Other(BaseItem *one)
{
	return m_from == one ? m_to : (
		m_to == one ? m_from : nullptr
	);
}

void Edge::DrawArrowHead(QPainter *painter) const
{
	assert(painter);
	painter->drawPolyline(
		QPolygonF{} << QPointF{arrow_width / 2, arrow_width} << QPointF{} << QPointF{-arrow_width / 2, arrow_width}
	);
}

void Edge::DrawAggregationDiamond(QPainter *painter) const
{
	assert(painter);
	painter->drawPolygon(
		QPolygonF{}
			<< QPointF{}
			<< QPointF{arrow_width / 2, arrow_width}
			<< QPointF{0, arrow_width * 2}
			<< QPointF{-arrow_width / 2, arrow_width},
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

/**
 * \brief Disconnect an item from the edge.
 *
 * This function is called when one of the BaseItem the edge is going to be destroyed.
 * Therefore we need to disconnect the edge from the dying item.
 * Actually, the edge will be destroyed very soon. There is technically no need to
 * disconnect. This function is called to eliminate the tiny chance between destroying
 * the dying item and destroying the edge, i.e. in the destructor of the edge, where
 * one of the item may be dangled.
 *
 * \param one
 */
void Edge::Disconnect(BaseItem *one)
{
	if (m_from == one) m_from = nullptr;
	else if (m_to == one) m_to = nullptr;
}
	
}} // end of namespace
