/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/4/17.
//

#pragma once

#include <QtWidgets/QDialog>

#include <memory>

namespace Ui {
class PreferenceBox;
}

namespace gui {

class PreferenceBox : public QDialog
{
public:
	PreferenceBox(QWidget *parent);
	~PreferenceBox();
	
private:
	std::unique_ptr<Ui::PreferenceBox> m_ui;
};
	
} // end of namespace
