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

#include "codebase/TypeRef.hh"

using namespace codebase;

class TypeRefTest : public testing::Test
{
protected:
	TypeRef m_base{"c:@S@Base"};
	TypeRef m_int{CXType_Int};
	TypeRef m_temp_inst{
		TypeRef{"c:@S@RecursiveBase>#$@S@Base"}.
		SetTemplateID("c:@ST>1#T@RecursiveBase").
		AddTempArgs(TypeRef{m_base})
	};
};

TEST_F(TypeRefTest, ID_only_constructor_construct_record_type)
{
	ASSERT_EQ(CXType_Record, m_base.Kind());
	ASSERT_EQ("Record", m_base.Name());
}

TEST_F(TypeRefTest, Class_ref_does_not_equal_to_build_ref)
{
	ASSERT_NE(m_int, m_base);
	ASSERT_NE(m_base, m_temp_inst);
}

TEST_F(TypeRefTest, Check_is_template)
{
	ASSERT_TRUE(m_temp_inst.IsTemplate());
	ASSERT_FALSE(m_int.IsTemplate());
	ASSERT_FALSE(m_base.IsTemplate());
}

TEST_F(TypeRefTest, Copy_ctor_makes_copy)
{
	ASSERT_EQ(m_temp_inst, TypeRef{m_temp_inst});
	ASSERT_EQ(m_int, TypeRef{m_int});
	ASSERT_EQ(m_base, TypeRef{m_base});
}