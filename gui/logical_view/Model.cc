/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "Model.hh"

#include "codebase/Entity.hh"
#include "codebase/DataType.hh"
#include "codebase/Namespace.hh"

#include <QtCore/QMimeData>
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtGui/QIcon>

#include <cassert>
#include <sstream>
#include <typeindex>

namespace gui {
namespace logical_view {

const QString Model::m_mime_type{"application/vnd.spag.usr"};

Model::Model(const codebase::Entity *root, const codebase::EntityMap *index, QObject *parent) :
	QAbstractItemModel{parent},
	m_root{root},
	m_index{index}
{
	assert(m_root);
	assert(m_index);
}

int Model::rowCount(const QModelIndex& parent) const
{
	return static_cast<int>(At(parent)->ChildCount());
}

const codebase::Entity *Model::At(const QModelIndex& idx) const
{
	return idx == QModelIndex{} ? m_root : reinterpret_cast<const codebase::Entity *>(idx.internalPointer());
}

QVariant Model::data(const QModelIndex& index, int role) const
{
	auto entity = At(index);
	assert(entity);
	
	static const std::map<std::type_index, QIcon> icons = {
		{typeid(const codebase::DataType&),  QIcon{":/images/class.png"}},
		{typeid(const codebase::Namespace&), QIcon{":/images/namespace.png"}},
	};
	
	switch (role)
	{
	case Qt::DisplayRole:    return QString::fromStdString(index.column() == 0 ? entity->Name() : entity->Type());
	case Qt::DecorationRole:
	{
		if (index.column() == 0)
		{
			auto iit = icons.find(typeid(*entity));
			if (iit != icons.end())
				return iit->second;
		}
	}
	default: break;
	}
	return {};
}

Qt::ItemFlags Model::flags(const QModelIndex& idx) const
{
	auto flag = QAbstractItemModel::flags(idx);
	if (idx != QModelIndex{})
		flag |= Qt::ItemIsDragEnabled;
	return flag;
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
		
	return {};
}

int Model::columnCount(const QModelIndex&) const
{
	return 2;
}

bool Model::hasChildren(const QModelIndex& parent) const
{
	return At(parent)->ChildCount() > 0;
}

QModelIndex Model::index(int row, int column, const QModelIndex& parent) const
{
	auto parent_entity = At(parent);
	assert(parent_entity);
	
	auto urow = static_cast<std::size_t>(row);
	
	return urow < parent_entity->ChildCount() ? createIndex(
		row,
		column,
		const_cast<codebase::Entity *>(parent_entity->Child(urow))
	) : QModelIndex{};
}

QModelIndex Model::parent(const QModelIndex& child) const
{
	auto pchild = At(child);
	assert(pchild);
	
	auto parent = m_index->Find(pchild->Parent());
	assert(parent);
	
	return pchild == parent ? QModelIndex{} : createIndex(
		static_cast<int>(parent->IndexOf(pchild)), 0,
		const_cast<codebase::Entity *>(parent)
	);
}

QMimeData *Model::mimeData(const QModelIndexList& ids) const
{
	std::ostringstream usrs;
	
	for (auto&& id : ids)
	{
		// the user always select the whole row, so all the model index of
		// the whole row will be included in "ids"
		// we only want one
		if (id.isValid() && id.column() == 0)
		{
			auto entity = At(id);
			assert(entity);
			usrs << entity->ID() << '\n';
		}
	}
	
	auto mime = new QMimeData;
	mime->setData(m_mime_type, usrs.str().c_str());
	return mime;
}
	
}} // end of namespace
