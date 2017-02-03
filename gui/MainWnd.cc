/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the Linux
    distribution for more details.
*/

//
// Created by nestal on 9/12/16.
//

#include "MainWnd.hh"
#include "Model.hh"

#include "ui_MainWnd.h"

#include <QFileDialog>
#include <QMessageBox>

#include <cassert>

namespace gui {

MainWnd::MainWnd() :
	m_ui{std::make_unique<Ui::MainWnd>()},
	m_model{std::make_unique<Model>(this)}
{
	// http://stackoverflow.com/questions/31522637/how-do-i-render-a-triangle-in-qopenglwidget
	// need to promote widget
	
	m_ui->setupUi(this);
	m_model->AttachView(m_ui->m_main);
		
	connect(m_ui->m_action_about, &QAction::triggered, [this]{QMessageBox::aboutQt(this);});
	connect(m_ui->m_action_open,  &QAction::triggered, [this]
	{
		assert(m_model);
		auto file = QFileDialog::getOpenFileName(this, tr("Open Source Code"));
		
		// string will be null if user press cancel
		if (!file.isNull())
			m_model->Parse(file);
	});
}

MainWnd::~MainWnd() = default;
	
} // end of namespace
