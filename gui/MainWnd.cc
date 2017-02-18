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
#include "gui/source_view/View.hh"

#include "ui_MainWnd.h"

#include "class_diagram/View.hh"
#include "gui/class_diagram/Model.hh"
#include "gui/source_view/Model.hh"
#include "libclx/Index.hh"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include <cassert>

namespace gui {

MainWnd::MainWnd() :
	m_ui{std::make_unique<Ui::MainWnd>()},
	m_doc{std::make_unique<Document>(this)}
{
	m_ui->setupUi(this);

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
	connect(m_ui->m_action_open,       &QAction::triggered, [this]
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
	
	// close widget when user clicks it
	connect(m_ui->m_tab, &QTabWidget::tabCloseRequested, [this](int tab)
	{
		auto w = m_ui->m_tab->widget(tab);
		m_ui->m_tab->removeTab(tab);
		delete w;
	});
	
	connect(m_ui->m_action_new_class_diagram, &QAction::triggered, this, &MainWnd::AddClassDiagram);

	// double click the tab to rename it
	connect(m_ui->m_tab->tabBar(), &QTabBar::tabBarDoubleClicked, this, &MainWnd::OnRenameTab);
	
	// default class diagram
	AddClassDiagram();
}

MainWnd::~MainWnd() = default;

void MainWnd::OnDoubleClickItem(const QModelIndex& idx)
{
	auto loc = m_doc->LocateEntity(idx);
	if (loc != libclx::SourceLocation{})
	{
		source_view::View *view{};
		auto filename = loc.Filename();
		
		// search for existing tab showing the file
		for (int i = 0; i < m_ui->m_tab->count(); ++i)
		{
			auto w = dynamic_cast<source_view::View *>(m_ui->m_tab->widget(i));
			if (w && w->Filename() == filename)
				view = w;
		}
		
		if (!view)
		{
			auto model = m_doc->CreateSourceModel(QString::fromStdString(filename));
			view = new source_view::View{model, m_ui->m_tab};
			
			view->Open(loc);
			m_ui->m_tab->addTab(view, QString::fromStdString(model->Name()));
		}
		else
		{
			unsigned line, column, offset;
			loc.Get(filename, line, column, offset);
			view->GoTo(line, column);
		}
		
		assert(view);
		m_ui->m_tab->setCurrentWidget(view);
		view->setFocus(Qt::OtherFocusReason);
	}
}

void MainWnd::AddClassDiagram()
{
	// spaghetti's first signal
	auto scene  = m_doc->CreateClassDiagram(tr("Class Diagram") + QString::number(m_ui->m_tab->count() + 1));
	auto view   = new class_diagram::View{scene, this};
	connect(view, &class_diagram::View::DropEntity, scene, &class_diagram::Model::AddEntity);
	
	auto tab = m_ui->m_tab->addTab(view, QString::fromStdString(scene->Name()));
	
	// after adding the view to the tab widget, it will be resized to fill the whole tab
	// we can use its size to resize the scene
	scene->SetRect(rect());
	
	m_ui->m_tab->setCurrentIndex(tab);
}

void MainWnd::OnRenameTab(int idx)
{
	if (auto view = dynamic_cast<common::ViewBase*>(m_ui->m_tab->widget(idx)))
	{
		auto model = view->Model();
		assert(model);
		
		if (model->CanRename())
		{
			bool ok;
			QString text = QInputDialog::getText(
				this, tr("Rename Tab"),
				tr("New name:"), QLineEdit::Normal,
				QString::fromStdString(model->Name()), &ok
			);
			
			if (ok && !text.isEmpty())
			{
				model->SetName(text);
				m_ui->m_tab->tabBar()->setTabText(idx, text);
			}
		}
	}
}

} // end of namespace
