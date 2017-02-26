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
#include "ProjectSetting.hh"

#include "ui_MainWnd.h"
#include "ui_AboutBox.h"

#include "gui/source_view/SourceModel.hh"
#include "libclx/Index.hh"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>

#include <cassert>

namespace gui {

static const QString file_dlg_filter{"JSON files (*.json)"};

class AboutDialog : public QDialog
{
public:
	AboutDialog(QWidget *parent) : QDialog{parent}, m_ui{std::make_unique<Ui::AboutDialog>()}
	{
		m_ui->setupUi(this);
		
		m_ui->m_about->setText(m_ui->m_about->text().arg(
			VERSION,
			BUILD_DATE
		));
		
		m_ui->m_version->setText(m_ui->m_version->text().arg(
			GIT_COMMIT_HASH,
			QT_VERSION_STR,
			CINDEX_VERSION_STRING,
			BOOST_LIB_VERSION
		));
	}

private:
	std::unique_ptr<Ui::AboutDialog> m_ui;
};

MainWnd::MainWnd() :
	m_ui{std::make_unique<Ui::MainWnd>()},
	m_doc{std::make_unique<Document>(this)}
{
	setWindowIcon(QIcon{":/images/fork2.svg"});
	m_ui->setupUi(this);
	m_ui->m_tab->Setup(*m_doc);
	
	// initialize logical view
	m_ui->m_logical_view->setModel(m_doc->ClassModel());
	m_ui->m_logical_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	
	// initialize project view
	m_ui->m_project_view->setModel(m_doc->ProjectModel());
	
	// other initializations
	m_doc->NewClassDiagram("Class Diagram");
	tabifyDockWidget(m_ui->m_project_dock, m_ui->m_logical_dock);
	setWindowTitle("Spaghetti : " + m_doc->Current());
	
	ConnectSignals();
}

MainWnd::~MainWnd() = default;

void MainWnd::ConnectSignals()
{
	connect(m_ui->m_action_about,    &QAction::triggered, [this]
	{
		AboutDialog dlg{this};
		dlg.exec();
	});
	connect(m_ui->m_action_new, &QAction::triggered, m_doc.get(), [this]{
		if (ConfirmDiscard())
			m_doc->New();
	});
	connect(m_ui->m_action_open, &QAction::triggered, this, &MainWnd::OnOpen);
	connect(m_ui->m_action_save_as,    &QAction::triggered, [this]
	{
		assert(m_doc);
		auto file = QFileDialog::getSaveFileName(this, tr("Save Project"), {}, file_dlg_filter);
		
		// string will be null if user press cancel
		if (!file.isNull())
			m_doc->SaveAs(file);
	});
	connect(m_ui->m_action_delete,     &QAction::triggered, m_ui->m_tab, &ViewSet::OnDelete );
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
	connect(m_ui->m_action_cflags, &QAction::triggered, [this]
	{
		ProjectSetting dlg{*m_doc, this};
		dlg.exec();
	});
	connect(m_ui->m_action_reload_all, &QAction::triggered, m_doc.get(), &Document::Reload);
	
	// open source code when the user double click the item
	connect(m_ui->m_logical_view, &QAbstractItemView::doubleClicked, this, &MainWnd::OnDoubleClickItem);
	
	connect(m_ui->m_action_new_class_diagram, &QAction::triggered, [this]
	{
		m_doc->NewClassDiagram(tr("Class Diagram") + QString::number(m_ui->m_tab->count() + 1));
	});
	connect(m_doc.get(), &Document::OnCompileDiagnotics, this, &MainWnd::Log);
	connect(m_doc.get(), &Document::OnSetCurrentFile, [this](auto& file){
		this->setWindowTitle("Spaghetti : " + file);}
	);
	connect(m_ui->m_logical_dock, &QDockWidget::visibilityChanged, [this](bool val){m_ui->m_action_logical_view->setChecked(val);});
	connect(m_ui->m_project_dock, &QDockWidget::visibilityChanged, [this](bool val){m_ui->m_action_project_view->setChecked(val);});
	connect(m_ui->m_log_dock, &QDockWidget::visibilityChanged, [this](bool val){m_ui->m_action_logs->setChecked(val);});
	connect(m_ui->m_action_logical_view, &QAction::triggered, [this]
	{
		m_ui->m_logical_dock->setVisible(!m_ui->m_logical_dock->isVisible());
		m_ui->m_action_logical_view->setChecked(m_ui->m_logical_dock->isVisible());
	});
	connect(m_ui->m_action_project_view, &QAction::triggered, [this]
	{
		m_ui->m_project_dock->setVisible(!m_ui->m_project_dock->isVisible());
		m_ui->m_action_project_view->setChecked(m_ui->m_project_dock->isVisible());
	});
	connect(m_ui->m_action_logs, &QAction::triggered, [this]
	{
		m_ui->m_log_dock->setVisible(!m_ui->m_log_dock->isVisible());
		m_ui->m_action_logs->setChecked(m_ui->m_log_dock->isVisible());
	});
}

void MainWnd::OnOpen()
{
	assert(m_doc);
	if (!ConfirmDiscard())
		return;
	
	auto file = QFileDialog::getOpenFileName(this, tr("Open Project"), {}, file_dlg_filter);

	// string will be null if user press cancel
	if (!file.isNull())
	{
		try
		{
			m_doc->Open(file);
		}
		catch (std::exception& e)
		{
			QMessageBox::critical(
				this, tr("Cannot open project"),
				tr("%1 is not a valid speghetti project file: %2").arg(file, e.what())
			);
		}
	}
}

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

bool MainWnd::ConfirmDiscard()
{
	if (m_doc->IsChanged())
	{
		QMessageBox msgBox;
		msgBox.setText("The document has been modified.");
		msgBox.setInformativeText("Do you want to discard your changes?");
		msgBox.setStandardButtons(QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		
		return msgBox.exec() == QMessageBox::Discard;
	}
	else
		return true;
}

void MainWnd::Log(const QString& message)
{
	m_ui->m_log_widget->appendPlainText(message);
}
	
} // end of namespace
