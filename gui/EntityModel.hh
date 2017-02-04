/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//


#pragma once

#include <QAbstractListModel>

namespace codebase {
class Entity;
}

namespace gui {

class EntityModel : public QAbstractItemModel
{
public:
	EntityModel(const codebase::Entity *root, QObject *parent);
	
	using QAbstractItemModel::beginResetModel;
	using QAbstractItemModel::endResetModel;

private:
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex&) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	bool hasChildren(const QModelIndex& parent) const override;
	
	QModelIndex index(int row, int column, const QModelIndex &parent) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	
private:
	const codebase::Entity* Get(const QModelIndex& idx) const;
	
private:
	const codebase::Entity *m_root{};
};
	
} // end of namespace