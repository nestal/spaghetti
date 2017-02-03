/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#pragma once

#include <QGraphicsItem>

#include "codebase/Class.hh"

class QGraphicsSimpleTextItem;

namespace gui {

class UMLClassItem : public QGraphicsItem
{
public:
	UMLClassItem(const codebase::Class& class_, QGraphicsItem *parent = {});
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
private:
	codebase::Class m_class;
	QRectF          m_bounding;
	
	QGraphicsSimpleTextItem *m_name;
};
	
} // end of namespace
