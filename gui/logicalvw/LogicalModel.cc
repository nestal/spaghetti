/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#include "LogicalModel.hh"

#include "codebase/Entity.hh"
#include "codebase/DataType.hh"
#include "codebase/Namespace.hh"
#include "codebase/Function.hh"
#include "codebase/Variable.hh"
#include "codebase/ClassTemplate.hh"

#include "gui/common/MimeType.hh"

#include <QtCore/QMimeData>
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtGui/QIcon>

#include <cassert>
#include <sstream>
#include <typeindex>

namespace gui {
namespace logicalvw {

LogicalModel::LogicalModel(const codebase::Entity *root, const codebase::EntityMap *index, QObject *parent) :
	QAbstractItemModel{parent},
	m_root{root},
	m_index{index}
{
	assert(m_root);
	assert(m_index);
}

int LogicalModel::rowCount(const QModelIndex& parent) const
{
	return static_cast<int>(At(parent)->ChildCount());
}

const codebase::Entity *LogicalModel::At(const QModelIndex& idx) const
{
	return idx == QModelIndex{} ? m_root : reinterpret_cast<const codebase::Entity *>(idx.internalPointer());
}

QVariant LogicalModel::data(const QModelIndex& index, int role) const
{
	auto entity = At(index);
	assert(entity);
	
	static const std::map<std::type_index, QIcon> icons = {
		{typeid(const codebase::DataType&), QIcon{":/images/class.png"}},
		{typeid(const codebase::Namespace&), QIcon{":/images/namespace.png"}},
		{typeid(const codebase::Function&), QIcon{":/images/function.png"}},
		{typeid(const codebase::Variable&), QIcon{":/images/variable.png"}},
		{typeid(const codebase::ClassTemplate&), QIcon{":/images/template.png"}},
		{typeid(const codebase::ClassTemplate::Instance&), QIcon{":/images/template2.png"}},
	};
	
	switch (role)
	{
	case Qt::DisplayRole: return QString::fromStdString(index.column() == 0 ? entity->Name() : entity->Type());
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

Qt::ItemFlags LogicalModel::flags(const QModelIndex& idx) const
{
	auto flag = QAbstractItemModel::flags(idx);
	if (idx != QModelIndex{})
		flag |= Qt::ItemIsDragEnabled;
	return flag;
}

QVariant LogicalModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int LogicalModel::columnCount(const QModelIndex&) const
{
	return 2;
}

bool LogicalModel::hasChildren(const QModelIndex& parent) const
{
	return At(parent)->ChildCount() > 0;
}

QModelIndex LogicalModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex LogicalModel::parent(const QModelIndex& child) const
{
	auto pchild = At(child);
	assert(pchild);
	
	auto parent = pchild->Parent();
	return parent == nullptr || parent == m_root ? QModelIndex{} : createIndex(
		static_cast<int>(parent->IndexOf(pchild)), 0,
		const_cast<codebase::Entity *>(parent)
	);
}

QMimeData *LogicalModel::mimeData(const QModelIndexList& ids) const
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
	mime->setData(mime::usr, usrs.str().c_str());
	return mime;
}

void LogicalModel::Reset(const codebase::Entity *root, const codebase::EntityMap *index)
{
	assert(root);
	assert(index);
	
	beginResetModel();
	m_root = root;
	m_index = index;
	endResetModel();
}
	
}} // end of namespace
