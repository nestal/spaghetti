/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/9/17.
//

#include "project/Project.hh"

#include <gtest/gtest.h>

using namespace project;

TEST(PROJECT_TEST, Test_Open_Project)
{
//	Project p;
//	p.Open(SRC_DIR, std::regex{".*\\.cc"});
}

TEST(PROJECT_TEST, Test_Open_Save)
{
	Project p;
	p.AddSource(SRC_DIR + std::string{"project/Project.cc"});
	p.Save("test_project");
	
	Project p2;
//	p2.Open("test_project");
//	ASSERT_EQ(p.CodeBase().Size(), p2.CodeBase().Size());
}