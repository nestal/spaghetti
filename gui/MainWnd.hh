/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 9/12/16.
//

#pragma once

#include <QtWidgets/QMainWindow>

#include <memory>

namespace Ui {
class MainWnd;
} // namespace Ui

class QGraphicsScene;

namespace gui {

class Document;

class MainWnd : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWnd();
	~MainWnd();

	void Log(const QString& message);
	void Open(const QString& file);
	
private:
	void ConnectSignals();
	void OnOpen();
	void OnSaveAs();
	void OnDoubleClickItem(const QModelIndex& idx);
	bool ConfirmDiscard();
	
	void closeEvent(QCloseEvent *event);

private:
	std::unique_ptr<Ui::MainWnd>    m_ui;
	std::unique_ptr<Document>       m_doc;
};
	
} // end of namespace
