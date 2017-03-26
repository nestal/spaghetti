/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

#include <gtest/gtest.h>

#include "Fixture.hh"

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/EntityMap.hh"


using namespace codebase;

class UniquePtrTest : public ut::Fixture
{
protected:
	UniquePtrTest() : Fixture{"unique_test.cc"}
	{
		
	}
};

TEST_F(UniquePtrTest, Test_unique_ptr_to_int)
{
	auto test_class = dynamic_cast<const DataType *>(m_map.FindByName("Test"));
	ASSERT_EQ("Test", test_class->Name());
	
	auto fields = test_class->Fields();
	ASSERT_EQ(1, fields.size());
	
	auto& member_i = fields.front();
	ASSERT_EQ("i", member_i.Name());
	
	auto unique_ptr_int = m_map.TypedFind<DataType>(member_i.TypeRef().ID());
	ASSERT_TRUE(unique_ptr_int);
	
	auto up_fields = unique_ptr_int->Fields();
	ASSERT_GE(up_fields.size(), 0);
}
