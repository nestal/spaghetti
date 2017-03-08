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

class ClassLayoutTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		
	}
	
protected:
	ItemRenderingOptions default_rendering_options;
	
	ClassLayout enough_space{
		"ClassName",
		"the_namespace::ClassName",
		{-100, -100, 200, 200},
		1.0,
		default_rendering_options,
		2,
		2
	};
	
	ClassLayout no_member{
		"ClassName",
		"the_namespace::ClassName",
		{-200, -200, 400, 400},
		1.0,
		default_rendering_options,
		0,
		0
	};
};

TEST_F(ClassLayoutTest, Use_default_margin_if_there_is_enough_space)
{
	ASSERT_EQ(QRectF(-90, -90, 180, 180), enough_space.ContentRect());
}

TEST_F(ClassLayoutTest, Use_font_size_if_there_is_enough_space)
{
	ASSERT_EQ(
		default_rendering_options.class_name_font.pointSizeF(),
		enough_space.NameFont().pointSizeF()
	);
	ASSERT_EQ(
		default_rendering_options.class_member_font.pointSizeF(),
		enough_space.NameFont().pointSizeF()
	);
}

TEST_F(ClassLayoutTest, Show_no_member_if_no_member)
{
	ASSERT_EQ(0, no_member.FieldCount());
	ASSERT_EQ(0, no_member.FunctionCount());
	ASSERT_TRUE(no_member.ContentRect().contains(no_member.NamePos()));
}
