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

#include "ModelViewFwd.hh"
#include "project/Project.hh"

#include <memory>

class QAbstractItemModel;
class QGraphicsScene;

namespace project {
class ModelBase;
}

namespace gui {

namespace logical_view {
class Model;
}

/**
 * \brief Aggregates all models in this application
 *
 * The Document contains all other models: class diagram models, logical view model,
 * and the Project instance. In other words, this class contains all data that we
 * wish to save to disk. Note that it does not include the code base, which is loaded
 * from disk separately.
 */
class Document : public QObject
{
	Q_OBJECT
	
public:
	Document(QObject *parent);
	~Document();

	void New();
	void Open(const QString& file);
	void SaveAs(const QString& file);
	
	void AddSource(const QString& file);
	
	// docking windows
	QAbstractItemModel* ClassModel();
	QAbstractItemModel* ProjectModel();
	
	void NewClassDiagram(const QString& name);
	void NewSourceView(const QString& name, unsigned line = 0, unsigned column = 0);
	
	libclx::SourceLocation LocateEntity(const QModelIndex& idx) const;
	
	void RemoveModel(project::ModelBase *model);
	
	bool IsChanged() const;

signals:
	void OnCreateClassDiagramView(class_diagram::Model *model);
	void OnCreateSourceView(source_view::Model *model);
	void OnDestroyModel(project::ModelBase *model);
	
private:
	void Reset(std::unique_ptr<project::Project>&& proj);
	
private:
	class ModelFactory;
	class ProjectModel_;
	
	std::unique_ptr<project::Project>       m_project;
	
	// for the docking windows
	std::unique_ptr<ProjectModel_>          m_project_model;
	std::unique_ptr<logical_view::Model>    m_logical_model;
};

} // end of namespace
