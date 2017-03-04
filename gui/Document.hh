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

#include <memory>
#include <codebase/CodeBase.hh>

class QAbstractItemModel;
class QGraphicsScene;

namespace project {
class Project;
class ModelBase;
}

namespace libclx {
class SourceLocation;
}

namespace gui {

namespace logicalvw {
class LogicalModel;
class ProxyModel;
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
	void Save();
	void SaveAs(const QString& file);
	
	void AddSource(const QString& file);
	QString CompileOptions() const;
	void SetCompileOptions(const QString& opts);
	QString ProjectDir() const;
	void SetProjectDir(const QString& dir);
	
	QString CompileDiagnotics() const;
	const QString& Current() const;
	
	// docking windows
	QAbstractItemModel* ClassModel();
	QAbstractItemModel* ProjectModel();
	const codebase::Entity *At(const QModelIndex& idx) const;
	
	void NewClassDiagram(const QString& name);
	void NewSourceView(const QString& name, unsigned line = 0, unsigned column = 0);
	
	libclx::SourceLocation LocateEntity(const QModelIndex& idx) const;
	
	void RemoveModel(project::ModelBase *model);
	
	bool IsChanged() const;
	void Reload();

	bool IsShowAllClasses() const;
	void SetShowAllClasses(bool value);
	
signals:
	void OnCreateClassDiagramView(classgf::ClassModel *model);
	void OnCreateSourceView(sourcevw::SourceModel *model);
	void OnDestroyModel(project::ModelBase *model);
	void OnCompileDiagnotics(const QString& line);
	void OnSetCurrentFile(const QString& file);
	void OnReload(const codebase::CodeBase& new_code_base);
	
private:
	void Reset(std::unique_ptr<project::Project>&& proj);
	void SetCurrentFile(const QString& file);
	
private:
	class ModelFactory;
	class ProjectModel_;
	
	QString                                     m_current_file;
	std::unique_ptr<project::Project>           m_project;
	
	// for the docking windows
	std::unique_ptr<ProjectModel_>              m_project_model;
	std::unique_ptr<logicalvw::LogicalModel>    m_logical_model;
	std::unique_ptr<logicalvw::ProxyModel>      m_proxy_model;
	
	bool m_changed{false};
};

} // end of namespace
