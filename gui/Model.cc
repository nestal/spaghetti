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
	QAbstractItemModel{parent},
	m_scene{std::make_unique<QGraphicsScene>(this)}
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
	
	m_codebase.Parse(file.toStdString());
	
	beginResetModel();
	
	auto dx = 0;
	for (auto& class_ : m_codebase)
	{
		auto item = new UMLClassItem{class_};
		item->moveBy(dx, 0);
		
		m_scene->addItem(item);
		dx += (item->boundingRect().width() + 10);
		
		m_classes.insert(item);
	}
	
	endResetModel();
}

void Model::AttachView(QGraphicsView *view)
{
	assert(view);
	view->setScene(m_scene.get());
}

int Model::rowCount(const QModelIndex& parent) const
{
	if (auto class_ = reinterpret_cast<const codebase::Class*>(parent.internalPointer()))
		return static_cast<int>(class_->FieldCount());
	else if (parent == QModelIndex{})
		return static_cast<int>(m_codebase.size());
	else
		return 0;
}

QVariant Model::data(const QModelIndex& index, int role) const
{
	auto entity = reinterpret_cast<const codebase::Entity*>(index.internalPointer());
	if (entity)
	{
		switch (role)
		{
		case Qt::DisplayRole:
			return QString::fromStdString(index.column() == 0 ? entity->Name() : entity->USR());
		default:
			break;
		}
	}

	return {};
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0: return tr("Name");
		case 1: return tr("Type");
		}
	}
	
	return QVariant();
}

int Model::columnCount(const QModelIndex&) const
{
	return 2;
}

bool Model::hasChildren(const QModelIndex& parent) const
{
	return parent == QModelIndex{} ||
		(parent.internalPointer() && dynamic_cast<const codebase::Class*>(static_cast<const codebase::Entity*>(parent.internalPointer())));
}

QModelIndex Model::index(int row, int column, const QModelIndex& parent) const
{
	auto urow = static_cast<std::size_t>(row);
	if (parent == QModelIndex() && urow < m_codebase.size())
	{
		return createIndex(row, column, const_cast<codebase::Class*>(&m_codebase.at(urow)));
	}
	else if (parent.parent() == QModelIndex())
	{
		auto prow = static_cast<std::size_t>(parent.row());
		return createIndex(row, column, const_cast<codebase::Class::Field*>(&m_codebase.at(prow).FieldAt(urow)));
	}
	else
		return {};
}

QModelIndex Model::parent(const QModelIndex& child) const
{
	if (child.internalPointer())
	{
		if (auto field = dynamic_cast<const codebase::Class::Field*>(static_cast<const codebase::Entity*>(child.internalPointer())))
		{
			auto it = m_codebase.find(field->Parent());
			if (it != m_codebase.end())
				return createIndex(m_codebase.IndexOf(it), 0, const_cast<codebase::Class*>(&*it));
		}
	}
	return {};
}

} // end of namespace
