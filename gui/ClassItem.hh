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

#include <QGraphicsWidget>

#include "codebase/Class.hh"

class QGraphicsTextItem;

namespace gui {

class ClassItem : public QGraphicsWidget
{
public:
	ClassItem(const codebase::Class& class_, QGraphicsItem *parent);
	
private:
	codebase::Class m_class;
	QRectF          m_rect;
};
	
} // end of namespace
