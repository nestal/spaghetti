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
#include "Edge.hh"
#include "codebase/DataType.hh"

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <cassert>

namespace gui {
namespace class_diagram {


Model::Model(const codebase::EntityMap *codebase, const QString& name, QObject *parent) :
	QObject{parent},
	m_name{name},
	m_scene{std::make_unique<QGraphicsScene>(this)},
	m_codebase{codebase}
{
	assert(m_codebase);
}

Model::~Model() = default;

void Model::SetRect(const QRectF& rect)
{
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
		
		// draw arrows
		DetectEdges(item);
		
		m_scene->addItem(item);
	}
}

QGraphicsScene *Model::Scene()
{
	return m_scene.get();
}

std::string Model::Name() const
{
	return m_name.toStdString();
}

void Model::SetName(const QString& name)
{
	m_name = name;
}

bool Model::CanRename() const
{
	return true;
}

/**
 * \brief Detects the relationship between a newly added item and the rest
 * \param item  the newly added item
 *
 * This function will add the edges between the new \a item and other
 * related ones.
 */
void Model::DetectEdges(ClassItem *item)
{
	for (auto child : m_scene->items())
	{
		if (auto citem = qgraphicsitem_cast<ClassItem*>(child))
		{
			if (item->DataType().IsBaseOf(citem->DataType()))
				AddLine(item, citem);
				
			else if (citem->DataType().IsBaseOf(item->DataType()))
				AddLine(citem, item);
		}
	}
}

void Model::AddLine(ClassItem *from, ClassItem *to)
{
	auto edge = std::make_unique<Edge>(from, to);
	from->AddEdge(edge.get());
	to->AddEdge(edge.get());
	m_scene->addItem(edge.release());
}

void Model::Load(const QJsonObject& obj)
{
	for (auto&& item_jval : obj["classes"].toArray())
	{
		auto json = item_jval.toObject();
		AddEntity(
			json["id"].toString().toStdString(),
			QPointF{
				json["x"].toDouble(),
				json["y"].toDouble()
			}
		);
	}
}

QJsonObject Model::Save() const
{
	QJsonArray items;
	for (auto child : m_scene->items())
	{
		if (auto citem = qgraphicsitem_cast<ClassItem*>(child))
			items.append(QJsonObject{
				{"id", QString::fromStdString(citem->DataType().ID())},
				{"x", citem->x()},
				{"y", citem->y()}
			});
	}
	
	return QJsonObject{{"classes", items}};
}
	
}} // end of namespace
