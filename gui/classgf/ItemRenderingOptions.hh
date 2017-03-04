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

#include <QtGui/QColor>
#include <QtGui/QFont>

namespace gui {
namespace classgf {

struct ItemRenderingOptions
{
	QColor line_color{Qt::GlobalColor::magenta};
	QColor class_box_color{Qt::GlobalColor::yellow};
	QColor class_box_color2{Qt::GlobalColor::white};
	QFont  class_name_font;
	QFont  class_member_font;
};
	
}} // end of namespace
