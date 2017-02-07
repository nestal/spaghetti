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
#include "Model.hh"

#include "ui_MainWnd.h"

#include "libclx/Index.hh"

#include <QFileDialog>
#include <QMessageBox>

#include <cassert>
#include <iostream>

namespace gui {

MainWnd::MainWnd() :
	m_ui{std::make_unique<Ui::MainWnd>()},
	m_model{std::make_unique<Model>(this)}
{
	m_ui->setupUi(this);
	m_model->AttachView(m_ui->m_class_gfx);
	
	// initialize tree view
	m_ui->m_class_tree->setModel(m_model->ClassModel());
	m_ui->m_class_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	
	connect(m_ui->m_action_about,    &QAction::triggered, [this]
	{
		QMessageBox::about(this,
			tr("About Spaghetti"),
			tr("Spaghetti: version 0.1\n"
			"License: GNU General Public License Version 2\n"
			"https://gitlab.com/nestal/spaghetti\n"
			"(C) 2017 Wan Wai Ho (Nestal)")
		);
	});
	connect(m_ui->m_action_about_Qt, &QAction::triggered, [this]{QMessageBox::aboutQt(this);});
	connect(m_ui->m_action_open,     &QAction::triggered, [this]
	{
		assert(m_model);
		auto file = QFileDialog::getOpenFileName(this, tr("Open Source Code"));
		
		// string will be null if user press cancel
		if (!file.isNull())
		{
			m_model->Parse(file);
		}
	});
	
	// open source code when the user double click the item
	connect(m_ui->m_class_tree, &QAbstractItemView::doubleClicked, [this](const QModelIndex& idx)
	{
		auto loc = m_model->LocateEntity(idx);
		if (loc != libclx::SourceLocation{})
			m_ui->m_code_view->Open(loc);
	});
	
	// spaghetti's first signal
	connect(m_ui->m_class_gfx, &ClassDiagramView::DropEntity, [this](const std::string& id, const QPointF& pos)
	{
		m_model->AddEntity(id, pos);
	});
}

MainWnd::~MainWnd() = default;
	
} // end of namespace
