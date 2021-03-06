/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/13/17.
//

#pragma once

#include "BaseItem.hh"
#include <QGraphicsItem>

namespace gui {
namespace classgf {

/**
 * \brief A connector between two items in the class diagram.
 *
 * An Edge connects two items in the class diagram. The connection is directional, meaning
 * that the two items being joined have different roles. The edge draws itself depending
 * on the roles of the two items being joined. If one item inherits the other, the edge draws
 * an UML inheritance arrow.
 */
class Edge : public BaseItem, public QGraphicsItem
{
public:
	Edge(BaseItem *from, BaseItem *to);
	
	QGraphicsItem *GraphicsItem() override;
	const QGraphicsItem *GraphicsItem() const override;
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	void UpdatePosition();
	
	ItemRelation RelationOf(const BaseItem *other) const override;
	classgf::ItemType ItemType() const override;
	void Update(const codebase::EntityMap *code_base) override ;
	
	bool IsChanged() const override;
	const BaseItem *Other(const BaseItem *one) const;
	BaseItem *Other(BaseItem *one);
	
	void Disconnect(BaseItem *one);
	
private:
	QLineF Dia() const;
	
	void DrawEndings(QPainter *painter, const QLineF& dia) const;
	void DrawToEnding(QPainter *painter, ItemRelation relation) const;
	void DrawFromEnding(QPainter *painter, ItemRelation relation) const;
	
	void DrawInheritanceTrigangle(QPainter *painter) const;
	void DrawAggregationDiamond(QPainter *painter) const;
	void DrawArrowHead(QPainter *painter) const;

private:
	BaseItem *m_from;
	BaseItem *m_to;
	
	// cached for performance
	QRectF m_bounding;
};
	
}} // end of namespace
