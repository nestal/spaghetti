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
#include "gui/common/MimeType.hh"
#include "codebase/DataType.hh"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QMimeData>
#include <QtGui/QPainter>
#include <QtCore/QBuffer>
#include <QtSvg/QSvgGenerator>

#include <QDebug>

#include <cassert>
#include <iostream>
#include "gui/common/CommonIO.hh"

namespace gui {
namespace classgf {

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
	AddItem(id, m_codebase, pos);
}

template <typename... Args>
void ClassModel::AddItem(Args&&... args)
{
	try
	{
		auto item = std::make_unique<ClassItem>(std::forward<Args>(args)...);
		connect(item.get(), &ClassItem::OnJustChanged, this, &ClassModel::OnChildChanged);
		
		// draw arrows
		DetectEdges(item.get());
		
		m_scene->addItem(item.release());
		
		// the model is changed because it has one more entity
		SetChanged(true);
	}
	catch (std::exception& e)
	{
		// TODO: print log
		QMessageBox::warning(nullptr, "Exception when adding item", e.what());
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
	
	for (auto&& item : obj["classes"].toArray())
		AddItem(item.toObject(), m_codebase);
	
	m_changed = false;
}

QJsonObject ClassModel::Save() const
{
	QJsonArray items;
	ForEachItem<ClassItem>(m_scene->items(), [this, &items](auto citem)
	{
		items.append(citem->Save());
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

QImage ClassModel::RenderImage(const QRectF& rect) const
{
	// Create the image with the exact size of the shrunk scene
	auto size = rect.isNull() ? m_scene->itemsBoundingRect().size().toSize() : rect.size().toSize();
	
	QImage image{size, QImage::Format_ARGB32};
	image.fill(Qt::transparent);
	
	QPainter painter(&image);
	m_scene->render(&painter, {}, rect);
	return image;
}

std::unique_ptr<QMimeData> ClassModel::CopySelection() const
{
	QRectF selected;
	for (auto&& item : m_scene->selectedItems())
		selected = selected.united(item->sceneBoundingRect());
	
	auto mime = std::make_unique<QMimeData>();
	mime->setImageData(RenderImage(selected));
	mime->setData(mime::svg, RenderSVG(selected));
	
	QJsonArray jarr;
	ForEachItem<ClassItem>(m_scene->items(), [this, &jarr](auto citem)
	{
		jarr.append(citem->Save());
	});
	mime->setData(mime::json, QJsonDocument{jarr}.toJson());
	
	return mime;
}

void ClassModel::Paste(const QMimeData* data)
{
	assert(data);
		
	if (data->hasFormat(mime::json))
	{
		auto json = QJsonDocument::fromJson(data->data(mime::json));
		for (auto&& item : json.array())
		{
			AddItem(item.toObject(), m_codebase);
			m_changed = true;
		}
	}
}

QByteArray ClassModel::RenderSVG(const QRectF& rect) const
{
	auto size = rect.isNull() ? m_scene->itemsBoundingRect().size().toSize() : rect.size().toSize();
	
	QBuffer b;
	QSvgGenerator p;
	p.setOutputDevice(&b);
	p.setSize(size);
	p.setViewBox(QRect{0, 0, size.width(), size.height()});
	
	{
		QPainter painter(&p);
		m_scene->render(&painter, {}, rect);
	}
	
	return b.buffer();
}

void ClassModel::AddParentClass(ClassItem *item, const QPointF& pos)
{
	if (!item && !m_scene->selectedItems().isEmpty())
		item = dynamic_cast<ClassItem*>(m_scene->selectedItems().front());
	
	if (item)
	{
		for (auto&& bases : item->DataType().BaseClasses())
			AddEntity(bases.ID(), pos);
	}
}
	
}} // end of namespace
