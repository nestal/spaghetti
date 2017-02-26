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

namespace codebase {
class CodeBase;
}

namespace gui {
namespace class_diagram {

class BaseItem : public QGraphicsItem
{
public:
	using QGraphicsItem::QGraphicsItem;
	
	virtual class_diagram::ItemType ItemType() const = 0;
	virtual ItemRelation RelationOf(const BaseItem *other) const = 0;
	virtual bool IsChanged() const = 0;
	virtual void Update(const codebase::CodeBase& code_base) = 0;
};

}} // end of namespace
