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

namespace gui {

Model::Model(QObject *parent) :
	QAbstractListModel{parent},
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
	return static_cast<int>(parent == QModelIndex() ? m_codebase.size() : ClassAt(parent)->FieldCount());
}

QVariant Model::data(const QModelIndex& index, int role) const
{
	if (index.row() >= 0 && static_cast<std::size_t>(index.row()) < m_codebase.size())
	{
		auto& class_ = m_codebase.at(static_cast<std::size_t>(index.row()));
		switch (role)
		{
		case Qt::DisplayRole:
			return QString::fromStdString(index.column() == 0 ? class_.Name() : class_.USR());
		default:
			break;
		}
	}
	
	return QVariant();
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

boost::optional<const codebase::Class&> Model::ClassAt(const QModelIndex& index) const
{
	auto row = static_cast<std::size_t>(index.row());
	return index.parent() == QModelIndex() && row < m_codebase.size() ?
		m_codebase.at(row) :
		boost::optional<const codebase::Class&>{};
}
	
} // end of namespace
