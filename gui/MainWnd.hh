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

#include <QMainWindow>

#include <memory>
#include <libclx/Index.hh>

namespace Ui {
class MainWnd;
} // namespace Ui

class QGraphicsScene;

namespace gui {

class ProjectModel;

class MainWnd : public QMainWindow
{
	Q_OBJECT

public:
	MainWnd();
	~MainWnd();

private:
	void OnDoubleClickItem(const QModelIndex& idx);
	
private:
	std::unique_ptr<Ui::MainWnd>    m_ui;
	std::unique_ptr<ProjectModel>          m_model;
};
	
} // end of namespace
