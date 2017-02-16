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

#include <QGraphicsItem>

namespace gui {
namespace class_diagram {

class Edge : public QGraphicsItem
{
public:
	Edge(const QGraphicsItem *from, const QGraphicsItem *to);
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	void UpdatePosition();
	
private:
	const QGraphicsItem *m_from;
	const QGraphicsItem *m_to;
	
	QRectF m_bounding;
};
	
}} // end of namespace
