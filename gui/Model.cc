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

#include <iterator>
#include <cassert>
#include <iostream>

namespace gui {

Model::Model(QObject *parent) :
	QObject{parent},
	m_scene{std::make_unique<QGraphicsScene>(this)},
	m_class_model{&m_codebase, &m_codebase, this}
{
}

void Model::Parse(const QString& file)
{
	// delete all items
	for (auto&& item : m_scene->items())
	{
		m_scene->removeItem(item);
		delete item;
	}
	
	m_class_model.beginResetModel();
	m_codebase.Parse(file.toStdString());

/*	auto dx = 0;
	for (std::size_t i = 0 ; i < m_codebase.ChildCount(); ++i)
	{
		auto item = new UMLClassItem{*m_codebase.Child(i)};
		item->moveBy(dx, 0);
		
		m_scene->addItem(item);
		dx += (item->boundingRect().width() + 10);
		
		m_classes.insert(item);
	}*/
	
	m_class_model.endResetModel();
}

void Model::AttachView(QGraphicsView *view)
{
	assert(view);
	view->setScene(m_scene.get());
}

QAbstractItemModel *Model::ClassModel()
{
	return &m_class_model;
}
	
} // end of namespace
