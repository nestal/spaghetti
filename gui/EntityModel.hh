/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//


#pragma once

#include <QAbstractListModel>

namespace codebase {
class Entity;
class EntityMap;
}

namespace gui {

/**
 * \brief Model for entity tree
 *
 * Used with Qt model-view architecture.
 */
class EntityModel : public QAbstractItemModel
{
public:
	EntityModel(const codebase::Entity *root, const codebase::EntityMap *index, QObject *parent);
	
	using QAbstractItemModel::beginResetModel;
	using QAbstractItemModel::endResetModel;
	
	const codebase::Entity* At(const QModelIndex& idx) const;

private:
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex&) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	bool hasChildren(const QModelIndex& parent) const override;
	
	QModelIndex index(int row, int column, const QModelIndex &parent) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	Qt::ItemFlags flags(const QModelIndex& idx) const override;
	
private:
	const codebase::Entity    *m_root{};
	const codebase::EntityMap *m_index{};
};
	
} // end of namespace
