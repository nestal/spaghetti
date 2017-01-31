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
#include <QtWidgets/QGraphicsView>

#include <cassert>

namespace gui {

Model::Model(QObject *parent, QGraphicsView *view) :
	QObject{parent},
	m_scene{std::make_unique<QGraphicsScene>(this)}
{
	assert(view);
	
//	m_codebase.Parse("codebase/CodeBase.cc");
	view->setScene(m_scene.get());
}

void Model::Parse(const QString& file)
{
	// delete all items
	for (auto&& item : m_scene->items())
	{
		m_scene->removeItem(item);
		delete item;
	}
	
	m_codebase.Parse(file.toStdString());

	auto dx = 0;
	for (auto& class_ : m_codebase)
	{
		auto item = new UMLClassItem{class_, nullptr};
		item->moveBy(dx, 0);
		
		m_scene->addItem(item);
		dx += (item->boundingRect().width() + 10);
	}
}
	
} // end of namespace
