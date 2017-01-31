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
#include "UMLClassItem.hh"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsView>

namespace gui {

Model::Model(QObject *parent, QGraphicsView *view) :
	QObject(parent),
	m_scene(std::make_unique<QGraphicsScene>(this))
{
	m_codebase.Parse("codebase/CodeBase.cc");
	
	auto dx = 0;
	for (auto& class_ : m_codebase)
	{
		auto item = new UMLClassItem{class_, nullptr};
		item->moveBy(dx, 0);
		
		m_scene->addItem(item);
		dx += (item->boundingRect().width() + 10);
	}
	
	view->setScene(m_scene.get());
}
	
}