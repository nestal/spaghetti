/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "EntityModel.hh"

#include "codebase/Entity.hh"

#include <cassert>

namespace gui {

EntityModel::EntityModel(const codebase::Entity *root, QObject *parent) :
	QAbstractItemModel{parent},
	m_root{root}
{
	assert(m_root);
}

int EntityModel::rowCount(const QModelIndex& parent) const
{
	return static_cast<int>(Get(parent)->ChildCount());
}

const codebase::Entity *EntityModel::Get(const QModelIndex& idx) const
{
	return idx == QModelIndex{} ? m_root : reinterpret_cast<const codebase::Entity*>(idx.internalPointer());
}

QVariant EntityModel::data(const QModelIndex& index, int role) const
{
	auto entity = Get(index);
	switch (role)
	{
	case Qt::DisplayRole:
		return QString::fromStdString(index.column() == 0 ? entity->Name() : entity->Type());
	default:
		break;
	}
	return {};
}

QVariant EntityModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int EntityModel::columnCount(const QModelIndex&) const
{
	return 2;
}

bool EntityModel::hasChildren(const QModelIndex& parent) const
{
	return Get(parent)->ChildCount() > 0;
}

QModelIndex EntityModel::index(int row, int column, const QModelIndex& parent) const
{
	auto parent_entity = Get(parent);
	return createIndex(row, column, const_cast<codebase::Entity*>(parent_entity->Child(static_cast<std::size_t>(row))));
}

QModelIndex EntityModel::parent(const QModelIndex& child) const
{
	auto pchild = Get(child);
	auto parent = pchild->Parent();
	return pchild == parent ? QModelIndex{} : createIndex(
		static_cast<int>(parent->IndexOf(pchild)), 0,
		const_cast<codebase::Entity*>(parent)
	);
}

} // end of namespace
