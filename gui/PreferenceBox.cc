/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/4/17.
//

#include "PreferenceBox.hh"

#include "ui_PreferenceBox.h"

namespace gui {

PreferenceBox::PreferenceBox(QWidget *parent) :
	QDialog{parent},
	m_ui{std::make_unique<Ui::PreferenceBox>()}
{
	m_ui->setupUi(this);
}

PreferenceBox::~PreferenceBox() = default;
	
} // end of namespace
