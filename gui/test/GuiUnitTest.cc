/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/8/17.
//

#include <QtGui/QGuiApplication>

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
