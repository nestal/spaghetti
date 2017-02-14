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

class ClassItem;

class Edge : public QGraphicsItem
{
public:
	Edge(const ClassItem *from, const ClassItem *to);
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	const ClassItem* Other(const ClassItem *one) const;
	
private:
	static QLineF LineFrom(const QRectF& from, const QRectF& to);

private:
	const ClassItem *m_from;
	const ClassItem *m_to;
};
	
}} // end of namespace
