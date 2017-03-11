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
#include <QtCore/QMimeData>
#include <QtGui/QPainter>
#include <QtCore/QBuffer>
#include <QtSvg/QSvgGenerator>

#include <QDebug>

#include <cassert>

namespace gui {
namespace classgf {

const qreal default_item_width{225.0}, default_item_height{175.0};

template <typename ItemType, typename Container, typename Func>
auto ForEachItem(Container&& cont, Func func)
{
	for (auto&& i : cont)
	{
		if (auto item = dynamic_cast<ItemType*>(i))
			func(item);
	}
	return func;
}

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
	AddEntityWithSize(id, pos, {default_item_width, default_item_height});
}

void ClassModel::AddEntityWithSize(const std::string& id, const QPointF& pos, const QSizeF& size)
{
	if (auto data_type = m_codebase->TypedFind<codebase::DataType>(id))
	{
		auto item = new ClassItem{*data_type, this, pos, size};
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
	ForEachItem<ClassItem>(m_scene->items(), [this, item](auto citem)
	{
		if (item->RelationOf(citem) != ItemRelation::no_relation && !item->HasEdgeWith(citem))
			this->AddLine(citem, item);
	});
}

void ClassModel::AddLine(ClassItem *from, ClassItem *to)
{
	auto edge = std::make_shared<Edge>(from, to);
	from->AddEdge(edge);
	to->AddEdge(edge);
	m_scene->addItem(edge.get());
}

void ClassModel::Load(const QJsonObject& obj)
{
	// prevent AddEntity() to emit OnChange()
	m_changed = true;
	
	for (auto&& item_jval : obj["classes"].toArray())
	{
		auto json = item_jval.toObject();
		AddEntityWithSize(
			json["id"].toString().toStdString(),
			QPointF{
				json["x"].toDouble(),
				json["y"].toDouble()
			},
			QSizeF{
				json["width"].toDouble(default_item_width),
				json["height"].toDouble(default_item_height)
			}
		);
	}
	m_changed = false;
}

QJsonObject ClassModel::Save() const
{
	QJsonArray items;
	ForEachItem<ClassItem>(m_scene->items(), [this, &items](auto citem)
	{
		auto size = citem->boundingRect().size();
		items.append(QJsonObject{
			{"id", QString::fromStdString(citem->DataType().ID())},
			{"x", citem->x()},
			{"y", citem->y()},
			{"width", size.width()},
			{"height", size.height()}
		});
		citem->MarkUnchanged();
	});
	
	SetChanged(false);
	
	return QJsonObject{{"classes", items}};
}

void ClassModel::DeleteSelectedItem()
{
	ForEachItem<ClassItem>(m_scene->selectedItems(), [this](auto dead_item)
	{
		this->DeleteItem(dead_item);
	});
}

void ClassModel::DeleteItem(ClassItem *dead_item)
{
	this->SetChanged(true);
	
	m_scene->removeItem(dead_item->GraphicsItem());
	delete dead_item;
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

void ClassModel::UpdateCodeBase(const codebase::EntityMap *codebase)
{
	ForEachItem<BaseItem>(m_scene->items(), [codebase](auto item)
	{
		item->Update(codebase);
	});
	
	// ClassItem::Update() will remove edges, need to add them back
	ForEachItem<ClassItem>(m_scene->items(), [this](auto item)
	{
		this->DetectEdges(item);
	});
}

std::unique_ptr<QMimeData> ClassModel::CopySelection()
{
	// Create the image with the exact size of the shrunk scene
	auto size = m_scene->itemsBoundingRect().size().toSize();
	auto rect = m_scene->selectionArea().boundingRect();
	
	auto mime = std::make_unique<QMimeData>();
	
	// render image
	{
		QImage image{size, QImage::Format_ARGB32};
		image.fill(Qt::transparent);
		
		QPainter painter(&image);
		m_scene->render(&painter, {}, rect);
		
		mime->setImageData(image);
	}
	
	// render SVG
	{
		QBuffer b;
		QSvgGenerator p;
		p.setOutputDevice(&b);
		p.setSize(size);
		p.setViewBox(QRect{0, 0, size.width(), size.height()});
		
		QPainter painter;
		painter.begin(&p);
		m_scene->render(&painter, {}, rect);
		painter.end();
		
		mime->setData("image/svg+xml", b.buffer());
	}
	return mime;
}
	
}} // end of namespace
