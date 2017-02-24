/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/21/17.
//

#include <QtWidgets/QFileDialog>
#include "ProjectSetting.hh"

#include "ui_ProjectSetting.h"

#include "Document.hh"

namespace gui {

ProjectSetting::ProjectSetting(Document& doc, QWidget *parent) :
	QDialog{parent},
	m_ui{std::make_unique<Ui::ProjectSetting>()},
	m_doc{doc}
{
	m_ui->setupUi(this);
	
	connect(m_ui->m_browse_project_dir, &QPushButton::clicked, [this]{
		auto dir = QFileDialog::getExistingDirectory(this, tr("Project Directory"), ".");
		if (!dir.isNull())
			m_ui->m_project_dir->setText(dir);
	});

	m_ui->m_project_dir->setText(m_doc.ProjectDir());
	m_ui->m_compiler_options->setPlainText(m_doc.CompileOptions());
}

ProjectSetting::~ProjectSetting() = default;

void ProjectSetting::accept()
{
	m_doc.SetProjectDir(m_ui->m_project_dir->text());
	m_doc.SetCompileOptions(m_ui->m_compiler_options->toPlainText());
	
	QDialog::accept();
}

} // end of namespace
