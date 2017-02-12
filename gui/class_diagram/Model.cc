/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#include "Model.hh"

#include "ClassItem.hh"
#include "codebase/DataType.hh"

#include <QtWidgets/QGraphicsScene>
#include <cassert>

namespace gui {
namespace class_diagram {


Model::Model(const codebase::EntityMap *codebase, QObject *parent) :
	QObject{parent},
	m_scene{std::make_unique<QGraphicsScene>(this)},
	m_codebase{codebase}
{
	assert(m_codebase);
}

Model::~Model() = default;

void Model::SetRect(const QRectF& rect)
{
//	assert(m_scene);
//	view->setScene(m_scene.get());
	m_scene->setSceneRect(rect);
}

void Model::Clear()
{
	// delete all items
	for (auto&& item : m_scene->items())
	{
		m_scene->removeItem(item);
		delete item;
	}
}

void Model::AddEntity(const std::string& id, const QPointF& pos)
{
	if (auto data_type = dynamic_cast<const codebase::DataType*>(m_codebase->Find(id)))
	{
		auto item = new ClassItem{*data_type};
		item->moveBy(pos.x(), pos.y());
		
		m_scene->addItem(item);
	}
}

QGraphicsScene *Model::Scene()
{
	return m_scene.get();
}

QString Model::Name() const
{
	return m_name;
}

void Model::SetName(const QString& name)
{
	m_name = name;
}

bool Model::CanRename() const
{
	return true;
}
	
}} // end of namespace
