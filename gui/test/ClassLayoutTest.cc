/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/8/17.
//


#include "gui/classgf/ClassLayout.hh"
#include "gui/classgf/ItemRenderingOptions.hh"
#include "gui/common/CommonIO.hh"

#include <QFont>
#include <QFontMetricsF>
#include <QTransform>

#include <gtest/gtest.h>

using namespace gui::classgf;

TEST(ClassLayoutTest, Use_default_margin_if_there_is_enough_space)
{
	ClassLayout subject{
		{-100, -100, 200, 200},
		{},
		{},
		2,
		2
	};
	
	ASSERT_EQ(QRectF(-90, -90, 180, 180), subject.ContentRect());
}