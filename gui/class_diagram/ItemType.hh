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

#include <QGraphicsItem>

namespace gui {
namespace class_diagram {

enum class ItemType : int
{
	base = QGraphicsItem::UserType,
	
	class_item,
	edge,
};

enum class ItemRelation
{
	no_relation,
	
	base_class_of,
	derived_class_of,
	use_as_member,
	used_by_as_member,
};

}} // end of namespace
