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

#include "gui/class_diagram/Model.hh"
#include "gui/source_view/Model.hh"
#include "logical_view/Model.hh"

#include <QtCore/QAbstractListModel>

#include <boost/filesystem.hpp>

#include <cassert>

namespace gui {

namespace fs = boost::filesystem;

class Document::ProjectModel_ : public QAbstractListModel
{
public:
	ProjectModel_(const codebase::CodeBase& codebase, QObject *parent) :
		QAbstractListModel{parent}, m_base{fs::absolute(".")}, m_codebase{codebase}
	{
	}
	
	int rowCount(const QModelIndex&) const override
	{
		return static_cast<int>(m_codebase.Size());
	}
	
	QVariant data(const QModelIndex& index, int role) const override
	{
		auto row = static_cast<std::size_t>(index.row());
		return role == Qt::DisplayRole && row < m_codebase.Size() ?
			QString::fromStdString(fs::path{m_codebase.At(row).Spelling()}.lexically_relative(m_base).string()) :
			QVariant{};
	}
	
	using QAbstractItemModel::beginResetModel;
	using QAbstractItemModel::endResetModel;

private:
	boost::filesystem::path     m_base;
	const codebase::CodeBase&   m_codebase;
};

Document::Document(QObject *parent) :
	QObject{parent},
	m_project_model{std::make_unique<ProjectModel_>(
		m_project.CodeBase(), this
	)},
	m_logical_model{std::make_unique<logical_view::Model>(
		m_project.CodeBase().Root(), &m_project.CodeBase(), this
	)}
{
}

Document::~Document() = default;

void Document::AddSource(const QString& file)
{
	m_logical_model->beginResetModel();
	m_project_model->beginResetModel();
	m_project.AddSource(file.toStdString());
	m_project_model->endResetModel();
	m_logical_model->endResetModel();
}

class_diagram::Model* Document::CreateClassDiagram(const QString& name)
{
	return static_cast<class_diagram::Model*>(
		m_project.Add(std::make_unique<class_diagram::Model>(&m_project.CodeBase(), name, this))
	);
}

source_view::Model *Document::CreateSourceModel(const QString& name)
{
	return static_cast<source_view::Model*>(
		m_project.Add(std::make_unique<source_view::Model>(name, this))
	);
}

QAbstractItemModel *Document::ClassModel()
{
	return m_logical_model.get();
}

libclx::SourceLocation Document::LocateEntity(const QModelIndex& idx) const
{
	auto entity = m_logical_model->At(idx);
	return entity ? entity->Location() : libclx::SourceLocation{};
}

void Document::Open(const QString& file)
{
	// delete all items
//	m_class_diagrams.front()->Clear();
	
	m_logical_model->beginResetModel();
	m_project_model->beginResetModel();
	
	try
	{
		m_project.Open(file.toStdString());
	}
	catch (std::exception&)
	{
		m_project_model->endResetModel();
		m_logical_model->endResetModel();
		throw;
	}
	m_project_model->endResetModel();
	m_logical_model->endResetModel();
}

void Document::SaveAs(const QString& file)
{
	m_project.Save(file.toStdString());
}

QAbstractItemModel* Document::ProjectModel()
{
	return m_project_model.get();
}

} // end of namespace
