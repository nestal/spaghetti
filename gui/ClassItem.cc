/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "ClassItem.hh"

namespace gui {

ClassItem::ClassItem(const codebase::Class& class_, QGraphicsItem *parent) :
	QGraphicsWidget{parent},
	m_class{class_}
{
	auto name = new QGraphicsTextItem{QString::fromStdString(m_class.Name()), this};

	// use a bold font for class names
	auto font = name->font();
	font.setBold(true);
	name->setFont(font);
	
	double ypos = name->boundingRect().height();
	for (auto& field : m_class.Fields())
	{
		auto field_item = new QGraphicsTextItem{QString::fromStdString(field.Name()), this};
		field_item->moveBy(0, ypos);
		ypos += field_item->boundingRect().height();
	}
	
	// add bounding rectangle
	new QGraphicsRectItem{boundingRect() | childrenBoundingRect(), this};
}

} // end of namespace
