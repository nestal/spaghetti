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

// gui namespace headers
#include "gui/class_diagram/ClassModel.hh"
#include "gui/source_view/SourceModel.hh"
#include "gui/logical_view/LogicalModel.hh"

#include "project/Project.hh"

#include <QtCore/QAbstractListModel>
#include <QtCore/QTextStream>
#include <QtWidgets/QApplication>

#include <boost/filesystem.hpp>

#include <cassert>

namespace gui {

namespace fs = boost::filesystem;

class Document::ProjectModel_ : public QAbstractListModel
{
public:
	ProjectModel_(const codebase::CodeBase* codebase, const fs::path& base, QObject *parent) :
		QAbstractListModel{parent}, m_base{base}, m_codebase{codebase}
	{
		assert(m_codebase);
	}
	
	void Reset(const codebase::CodeBase* codebase, const fs::path& base)
	{
		assert(m_codebase);
		
		beginResetModel();
		m_codebase = codebase;
		m_base     = base;
		endResetModel();
	}
	
	int rowCount(const QModelIndex&) const override
	{
		return static_cast<int>(m_codebase->Size());
	}
	
	QVariant data(const QModelIndex& index, int role) const override
	{
		auto row = static_cast<std::size_t>(index.row());
		return role == Qt::DisplayRole && row < m_codebase->Size() ?
			QString::fromStdString(relative(m_codebase->At(row).Spelling(), m_base).string()) :
			QVariant{};
	}
	
	using QAbstractItemModel::beginResetModel;
	using QAbstractItemModel::endResetModel;

private:
	fs::path                   m_base;
	const codebase::CodeBase   *m_codebase;
};

class Document::ModelFactory : public project::ModelFactory
{
public:
	ModelFactory(Document *parent) : m_parent{parent} {}
	project::Model Create(project::ModelType type, const std::string& name, project::Project& owner) override;

private:
	Document *m_parent;
};

Document::Document(QObject *parent) :
	QObject{parent},
	m_project{std::make_unique<project::Project>()},
	m_project_model{std::make_unique<ProjectModel_>(
		&m_project->CodeBase(), m_project->ProjectDir(), this
	)},
	m_logical_model{std::make_unique<logical_view::LogicalModel>(
		m_project->CodeBase().Root(), &m_project->CodeBase(), this
	)}
{
	SetCurrentFile(tr("Untitled"));
}

Document::~Document() = default;

void Document::AddSource(const QString& file)
{
	m_logical_model->beginResetModel();
	m_project_model->beginResetModel();
	m_project->AddSource(file.toStdString());
	m_project_model->endResetModel();
	m_logical_model->endResetModel();
}

void Document::NewClassDiagram(const QString& name)
{
	auto m = std::make_unique<class_diagram::ClassModel>(&m_project->CodeBase(), name, this);
	emit OnCreateClassDiagramView(m.get());
	m_project->Add(std::move(m));
}

void Document::NewSourceView(const QString& name, unsigned line, unsigned column)
{
	auto m = std::make_unique<source_view::SourceModel>(name, this);
	emit OnCreateSourceView(m.get());
	
	m->SetLocation(name, line, column);
	m_project->Add(std::move(m));
}

logical_view::LogicalModel *Document::ClassModel()
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
	try
	{
		auto proj = std::make_unique<project::Project>();
		
		QApplication::setOverrideCursor(Qt::WaitCursor);
		ModelFactory factory{this};
		proj->Open(file.toStdString(), factory);
		QApplication::restoreOverrideCursor();
		
		for (auto&& tu : proj->CodeBase().TranslationUnits())
			for (auto&& diag : tu.Diagnostics())
				emit OnCompileDiagnotics(QString::fromStdString(diag.Str()));
		
		Reset(std::move(proj));
		SetCurrentFile(file);
	}
	catch (std::exception&)
	{
		QApplication::restoreOverrideCursor();
		throw;
	}
}

void Document::SaveAs(const QString& file)
{
	m_project->Save(file.toStdString());
	SetCurrentFile(file);
}

QAbstractItemModel* Document::ProjectModel()
{
	return m_project_model.get();
}

project::Model Document::ModelFactory::Create(project::ModelType type, const std::string& name, project::Project& owner)
{
	project::Model result;
	
	switch (type)
	{
	case project::ModelType::class_diagram:
	{
		auto m = std::make_unique<class_diagram::ClassModel>(&owner.CodeBase(), QString::fromStdString(name), m_parent);
		emit m_parent->OnCreateClassDiagramView(m.get());
		result = std::move(m);
		break;
	}
	case project::ModelType::source_view:
	{
		auto m = std::make_unique<source_view::SourceModel>(QString::fromStdString(name), m_parent);
		emit m_parent->OnCreateSourceView(m.get());
		result = std::move(m);
		break;
	}
	default:
		assert(false);
	}
	
	return result;
}

void Document::RemoveModel(project::ModelBase *model)
{
	emit OnDestroyModel(model);
	m_project->Erase(model);
}

void Document::New()
{
	// don't destroy the origin project yet, because some models may still be referring to it
	Reset(std::make_unique<project::Project>());
	SetCurrentFile(tr("Untitled"));
	
	NewClassDiagram("Class Diagram");
}

void Document::Reset(std::unique_ptr<project::Project>&& proj)
{
	auto p = std::move(proj);
	swap(m_project, p);
	
	m_project_model->Reset(&m_project->CodeBase(), m_project->ProjectDir());
	m_logical_model->Reset(m_project->CodeBase().Root(), &m_project->CodeBase());
	
	// destroy old project by unique_ptr destructor
	for (std::size_t i = 0 ; p && i < p->Count() ; i++)
		emit OnDestroyModel(p->At(i));
}

bool Document::IsChanged() const
{
	assert(m_project);
	for (std::size_t i = 0 ; i < m_project->Count() ; i++)
		if (m_project->At(i)->IsChanged())
			return true;
	
	return false;
}

QString Document::CompileOptions() const
{
	assert(m_project);
	
	auto cflags = QString{};
	for (auto&& flag : m_project->CompileOptions())
		cflags += QString::fromStdString(flag) + " ";
	return cflags;
}

void Document::SetCompileOptions(const QString& opts)
{
	std::vector<std::string> cflags;
	for (auto&& flags : opts.split(" ", QString::SkipEmptyParts))
		cflags.push_back(flags.toStdString());
	m_project->SetCompileOptions(cflags.begin(), cflags.end());
}

QString Document::ProjectDir() const
{
	return QString::fromStdString(m_project->ProjectDir());
}

void Document::SetProjectDir(const QString& dir)
{
	m_project->SetProjectDir(dir.toStdString());
}

QString Document::CompileDiagnotics() const
{
	QString result;
	QTextStream str{&result};
	
	for (auto&& tu : m_project->CodeBase().TranslationUnits())
		for (auto&& diag : tu.Diagnostics())
			str << diag.Str().c_str();
	
	return result;
}

const QString& Document::Current() const
{
	return m_current_file;
}

void Document::SetCurrentFile(const QString& file)
{
	m_current_file = QString::fromStdString(fs::path{file.toStdString()}.filename().string());
	emit OnSetCurrentFile(m_current_file);
}

/**
 * \brief Re-compile all source files and update the models
 *
 * It behaves like Open(Current()), but it doesn't re-read from the JSON file.
 */
void Document::Reload()
{
	// TODO: implement it
}

} // end of namespace
