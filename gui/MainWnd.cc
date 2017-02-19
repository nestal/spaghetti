/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 9/12/16.
//

#include "MainWnd.hh"
#include "Document.hh"

#include "ui_MainWnd.h"

#include "class_diagram/Model.hh"
#include "source_view/Model.hh"
#include "libclx/Index.hh"

#include <QFileDialog>
#include <QMessageBox>

#include <cassert>

namespace gui {

MainWnd::MainWnd() :
	m_ui{std::make_unique<Ui::MainWnd>()},
	m_doc{std::make_unique<Document>(this)}
{
	m_ui->setupUi(this);
	m_ui->m_tab->Setup(*m_doc);

	// initialize logical view
	m_ui->m_logical_view->setModel(m_doc->ClassModel());
	m_ui->m_logical_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	
	// initialize project view
	m_ui->m_project_view->setModel(m_doc->ProjectModel());
	
	connect(m_ui->m_action_about,    &QAction::triggered, [this]
	{
		QMessageBox::about(this,
			tr("About Spaghetti"),
			tr("Spaghetti: version " VERSION " (" BUILD_DATE ")\n"
			"Git commit: " GIT_COMMIT_HASH "\n"
			"License: GNU General Public License Version 2\n"
			"https://gitlab.com/nestal/spaghetti\n"
			"(C) 2017 Wan Wai Ho (Nestal)")
		);
	});
	connect(m_ui->m_action_new, &QAction::triggered, m_doc.get(), &Document::New);
	connect(m_ui->m_action_open, &QAction::triggered, [this]
	{
		assert(m_doc);
		auto file = QFileDialog::getOpenFileName(this, tr("Open Project"));
		
		// string will be null if user press cancel
		if (!file.isNull())
		{
			try
			{
				m_doc->Open(file);
			}
			catch (std::exception& e)
			{
				QMessageBox::critical(this, tr("Cannot open project"),
					tr("%1 is not a valid speghetti project file: %2").arg(file, e.what())
				);
			}
			
		}
	});
	connect(m_ui->m_action_save_as,    &QAction::triggered, [this]
	{
		assert(m_doc);
		auto file = QFileDialog::getSaveFileName(this, tr("Save Project"), {}, tr("JSON files (*.json)"));
		
		// string will be null if user press cancel
		if (!file.isNull())
			m_doc->SaveAs(file);
	});
	connect(m_ui->m_action_about_Qt,   &QAction::triggered, [this]{QMessageBox::aboutQt(this);});
	connect(m_ui->m_action_add_source, &QAction::triggered, [this]
	{
		assert(m_doc);
		auto file = QFileDialog::getOpenFileName(this, tr("Add Source to Project"));
		
		// string will be null if user press cancel
		if (!file.isNull())
		{
			m_doc->AddSource(file);
		}
	});
	
	// open source code when the user double click the item
	connect(m_ui->m_logical_view, &QAbstractItemView::doubleClicked, this, &MainWnd::OnDoubleClickItem);
	
	connect(m_ui->m_action_new_class_diagram, &QAction::triggered, [this]
	{
		m_doc->NewClassDiagram(tr("Class Diagram") + QString::number(m_ui->m_tab->count() + 1));
	});
}

MainWnd::~MainWnd() = default;

void MainWnd::OnDoubleClickItem(const QModelIndex& idx)
{
	auto loc = m_doc->LocateEntity(idx);
	if (loc != libclx::SourceLocation{})
	{
		std::string filename;
		unsigned line, column, offset;
		loc.Get(filename, line, column, offset);
		
		m_ui->m_tab->ViewCode(filename, line, column);
	}
}
	
} // end of namespace
