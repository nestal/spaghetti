/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#include "ClassModel.hh"

#include "ClassItem.hh"
#include "Edge.hh"
#include "codebase/DataType.hh"

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <cassert>
#include <iostream>

namespace gui {
namespace class_diagram {


ClassModel::ClassModel(const codebase::EntityMap *codebase, const QString& name, QObject *parent) :
	QObject{parent},
	m_name{name},
	m_scene{std::make_unique<QGraphicsScene>(this)},
	m_codebase{codebase}
{
	assert(m_codebase);
}

ClassModel::~ClassModel() = default;

void ClassModel::SetRect(const QRectF& )
{
}

void ClassModel::Clear()
{
	// delete all items
	for (auto&& item : m_scene->items())
	{
		m_scene->removeItem(item);
		delete item;
	}
}

void ClassModel::AddEntity(const std::string& id, const QPointF& pos)
{
	if (auto data_type = dynamic_cast<const codebase::DataType*>(m_codebase->Find(id)))
	{
		auto item = new ClassItem{*data_type, pos, this};
		connect(item, &ClassItem::OnJustChanged, this, &ClassModel::OnChildChanged);
		
		// draw arrows
		DetectEdges(item);
		
		m_scene->addItem(item);
		
		// the model is changed because it has one more entity
		SetChanged(true);
	}
}

QGraphicsScene *ClassModel::Scene()
{
	return m_scene.get();
}

std::string ClassModel::Name() const
{
	return m_name.toStdString();
}

void ClassModel::SetName(const QString& name)
{
	m_name = name;
}

bool ClassModel::CanRename() const
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
void ClassModel::DetectEdges(ClassItem *item)
{
	for (auto child : m_scene->items())
	{
		if (auto citem = qgraphicsitem_cast<ClassItem*>(child))
		{
			if (item->RelationOf(citem) != ItemRelation::no_relation)
				AddLine(citem, item);
		}
	}
}

void ClassModel::AddLine(ClassItem *from, ClassItem *to)
{
	auto edge = std::make_unique<Edge>(from, to);
	from->AddEdge(edge.get());
	to->AddEdge(edge.get());
	m_scene->addItem(edge.release());
}

void ClassModel::Load(const QJsonObject& obj)
{
	// prevent AddEntity() to emit OnChange()
	m_changed = true;
	
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
	m_changed = false;
}

QJsonObject ClassModel::Save() const
{
	QJsonArray items;
	for (auto child : m_scene->items())
	{
		if (auto citem = qgraphicsitem_cast<ClassItem*>(child))
		{
			items.append(QJsonObject{
				{"id", QString::fromStdString(citem->DataType().ID())},
				{"x", citem->x()},
				{"y", citem->y()}
			});
			citem->MarkUnchanged();
		}
	}
	
	SetChanged(false);
	
	return QJsonObject{{"classes", items}};
}

void ClassModel::DeleteSelectedItem()
{
	for (auto&& item : m_scene->selectedItems())
	{
		SetChanged(true);
		
		m_scene->removeItem(item);
		delete dynamic_cast<BaseItem*>(item);
	}
}

bool ClassModel::IsChanged() const
{
	return m_changed;
}

void ClassModel::OnChildChanged(BaseItem *)
{
	SetChanged(true);
}

void ClassModel::SetChanged(bool changed) const
{
	if (m_changed != changed)
	{
		emit OnChanged(changed);
		m_changed = changed;
	}
}
	
}} // end of namespace
