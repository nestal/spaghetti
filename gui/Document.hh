/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 9/16/16.
//

#pragma once

// base class includes first
#include <QtCore/QObject>

#include "project/Project.hh"

#include <memory>

class QAbstractItemModel;
class QGraphicsScene;
class QGraphicsView;

namespace gui {
namespace class_diagram {
class ClassItem;
}

namespace logical_view {
class Model;
}

/**
 * \brief Aggregates all models in this application
 *
 * The Document contains all other models: class diagram models, logical view model,
 * and the Project instance.
 */
class Document : public QObject
{
public:
	Document(QObject *parent);
	~Document();

	void Open(const QString& file);
	void SaveAs(const QString& file);
	
	void AttachView(QGraphicsView *view);
	void AddSource(const QString& file);
	
	QAbstractItemModel* ClassModel();
	QAbstractItemModel* ProjectModel();
	
	libclx::SourceLocation LocateEntity(const QModelIndex& idx) const;
	void AddEntity(const std::string& id, const QPointF& pos);
	
private:
	// order is important here, since m_scene depends on m_repo.
	// m_scene contains CommitItem, which contains Commits. It must be destroyed
	// before the Repository is destroyed.
	project::Project                m_project;
	std::unique_ptr<QGraphicsScene> m_scene;

	class ProjectModel_;
	std::unique_ptr<ProjectModel_>  m_project_model;
	
	std::unique_ptr<logical_view::Model> m_class_model;
};
	
} // end of namespace
