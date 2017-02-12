/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#include "SceneModel.hh"

#include "ClassItem.hh"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <cassert>

namespace gui {
namespace class_diagram {


SceneModel::SceneModel(QObject *parent) :
	QObject{parent},
	m_scene{std::make_unique<QGraphicsScene>(this)}
{
}

SceneModel::~SceneModel() = default;

void SceneModel::AttachView(QGraphicsView *view)
{
	assert(m_scene);
	view->setScene(m_scene.get());
	m_scene->setSceneRect(view->rect());
}

void SceneModel::Clear()
{
	// delete all items
	for (auto&& item : m_scene->items())
	{
		m_scene->removeItem(item);
		delete item;
	}
}

void SceneModel::AddDataType(const codebase::DataType& type, const QPointF& pos)
{
	auto item = new class_diagram::ClassItem{type};
	item->moveBy(pos.x(), pos.y());
	
	m_scene->addItem(item);
}
	
}} // end of namespace
