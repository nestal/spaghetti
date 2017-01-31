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
	m_class{class_},
	m_name{new QGraphicsTextItem{QString::fromStdString(m_class.Name()), this}}
{
}

} // end of namespace
