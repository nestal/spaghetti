/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/17/17.
//

#pragma once

#include "ItemType.hh"
#include <QGraphicsItem>

namespace gui {
namespace class_diagram {

class BaseItem : public QGraphicsItem
{
public:
	using QGraphicsItem::QGraphicsItem;
};

}} // end of namespace
