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

namespace gui {

MainWnd::MainWnd() :
	m_ui(std::make_unique<Ui::MainWnd>())
{
	// http://stackoverflow.com/questions/31522637/how-do-i-render-a-triangle-in-qopenglwidget
	// need to promote widget
	
	m_ui->setupUi(this);
	
	connect(m_ui->m_action_about, &QAction::triggered, [this]{QMessageBox::aboutQt(this);});
	connect(m_ui->m_action_open,  &QAction::triggered, [this]
	{
		m_model = std::make_unique<Model>(
//			QFileDialog::getExistingDirectory(this, tr("Open Repository")).toStdString(),
			this,
			m_ui->m_main
		);
	});
}

MainWnd::~MainWnd() = default;
	
} // end of namespace
