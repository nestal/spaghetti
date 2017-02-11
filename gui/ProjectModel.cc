/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 9/16/16.
//

#include "ProjectModel.hh"
#include "UMLClassItem.hh"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

#include <cassert>

namespace gui {

ProjectModel::ProjectModel(QObject *parent) :
	QObject{parent},
	m_scene{std::make_unique<QGraphicsScene>(this)},
	m_class_model{m_codebase.Root(), &m_codebase, this}
{
}

ProjectModel::~ProjectModel() = default;

void ProjectModel::Parse(const QString& file)
{
	// delete all items
	for (auto&& item : m_scene->items())
	{
		m_scene->removeItem(item);
		delete item;
	}
	
	m_class_model.beginResetModel();
	m_codebase.Parse(file.toStdString(), {
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", SRC_DIR,
		"-DSRC_DIR=" + std::string{SRC_DIR}
	});
	m_class_model.endResetModel();
}

void ProjectModel::AttachView(QGraphicsView *view)
{
	assert(view);
	view->setScene(m_scene.get());
	m_scene->setSceneRect(view->rect());
}

QAbstractItemModel *ProjectModel::ClassModel()
{
	return &m_class_model;
}

libclx::SourceLocation ProjectModel::LocateEntity(const QModelIndex& idx) const
{
	auto entity = m_class_model.At(idx);
	return entity ? entity->Location() : libclx::SourceLocation{};
}

void ProjectModel::AddEntity(const std::string& id, const QPointF& pos)
{
	if (auto data_type = dynamic_cast<const codebase::DataType*>(m_codebase.Find(id)))
	{
		auto item = new UMLClassItem{*data_type};
		item->moveBy(pos.x(), pos.y());
		
		m_scene->addItem(item);
	}
}
	
} // end of namespace
