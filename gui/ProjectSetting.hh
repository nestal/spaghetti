/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/21/17.
//

#pragma once

#include <QtWidgets/QDialog>

#include <memory>

namespace Ui {
class ProjectSetting;
} // namespace Ui

namespace gui {

class Document;

class ProjectSetting : public QDialog
{
public:
	ProjectSetting(Document& doc, QWidget *parent);
	~ProjectSetting();
	
	void accept() override;
	
private:
	std::unique_ptr<Ui::ProjectSetting> m_ui;
	Document& m_doc;
};
	
} // end of namespace
