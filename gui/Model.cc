/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the Linux
    distribution for more details.
*/

//
// Created by nestal on 9/16/16.
//

#include "Model.hh"
#include "ClassItem.hh"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsView>

namespace gui {

Model::Model(QObject *parent, QGraphicsView *view) :
	QObject(parent),
	m_scene(std::make_unique<QGraphicsScene>(this))
{
	m_codebase.Parse("codebase/CodeBase.cc");
	
	for (auto& class_ : m_codebase)
	{
		auto item = new ClassItem{class_, nullptr};
		item->setFlag(QGraphicsItem::ItemIsMovable);
		item->setFlag(QGraphicsItem::ItemIsSelectable);
		
		m_scene->addItem(item);
	}
	
	view->setScene(m_scene.get());
}
	
}