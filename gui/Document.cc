/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 9/16/16.
//

#include "Document.hh"
#include "class_diagram/SceneModel.hh"
#include "logical_view/Model.hh"

#include <QtCore/QAbstractListModel>
#include <QtWidgets/QFileDialog>

#include <cassert>

namespace gui {

class Document::ProjectModel_ : public QAbstractListModel
{
public:
	ProjectModel_(const codebase::CodeBase& codebase, QObject *parent) :
		QAbstractListModel{parent}, m_codebase{codebase} {}
	
	int rowCount(const QModelIndex&) const override
	{
		return static_cast<int>(m_codebase.Size());
	}
	
	QVariant data(const QModelIndex& index, int role) const override
	{
		auto row = static_cast<std::size_t>(index.row());
		return role == Qt::DisplayRole && row < m_codebase.Size() ?
			QString::fromStdString(m_codebase.At(row).Spelling()) : QVariant{};
	}
	
	using QAbstractItemModel::beginResetModel;
	using QAbstractItemModel::endResetModel;

private:
	const codebase::CodeBase&   m_codebase;
};

Document::Document(QObject *parent) :
	QObject{parent},
	m_project_model{std::make_unique<ProjectModel_>(m_project.CodeBase(), this)},
	m_class_model{std::make_unique<logical_view::Model>(
		m_project.CodeBase().Root(), &m_project.CodeBase(), this
	)}
{
}

Document::~Document() = default;

void Document::AddSource(const QString& file)
{
	m_class_model->beginResetModel();
	m_project_model->beginResetModel();
	m_project.AddSource(file.toStdString());
	m_project_model->endResetModel();
	m_class_model->endResetModel();
}

class_diagram::SceneModel* Document::NewClassDiagram()
{
	m_classes.emplace_back(
		std::make_unique<class_diagram::SceneModel>(&m_project.CodeBase(), this)
	);
	return m_classes.back().get();
}

QAbstractItemModel *Document::ClassModel()
{
	return m_class_model.get();
}

libclx::SourceLocation Document::LocateEntity(const QModelIndex& idx) const
{
	auto entity = m_class_model->At(idx);
	return entity ? entity->Location() : libclx::SourceLocation{};
}

void Document::Open(const QString& file)
{
	// delete all items
	m_classes.front()->Clear();
	
	m_class_model->beginResetModel();
	m_project_model->beginResetModel();
	m_project.Open(file.toStdString());
	m_project_model->endResetModel();
	m_class_model->endResetModel();
}

void Document::SaveAs(const QString& file)
{
	m_project.Save(file.toStdString());
}

QAbstractItemModel *Document::ProjectModel()
{
	return m_project_model.get();
}
	
} // end of namespace
