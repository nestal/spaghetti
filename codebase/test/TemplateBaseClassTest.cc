/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/11/17.
//

#include "Fixture.hh"

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/EntityMap.hh"

#include <gtest/gtest.h>

using namespace codebase;

class TemplateBaseClassTest : public ut::Fixture
{
protected:
	TemplateBaseClassTest() : Fixture{"template_base.cc"}
	{
		
	}
};

TEST_F(TemplateBaseClassTest, Test_base_class)
{
	auto derived = dynamic_cast<const DataType *>(m_subject.Map().FindByName("Derived"));
	ASSERT_EQ("Derived", derived->Name());
	
	// base should be c:@S@RecursiveBase>#$@S@Base, but we need to fix it by
	// differentiating between a template and its instantiation
	std::vector<TypeRef> base{
		TypeRef{"c:@S@RecursiveBase>#$@S@Base", CXType_Unexposed}.SetTemplateID("c:@ST>1#T@RecursiveBase").AddTempArgs(TypeRef{"c:@S@Base"}),
		TypeRef{"c:@S@Base2"},
		TypeRef{"c:@S@Base3"}
	};
	ASSERT_EQ(base, derived->BaseClasses());
	
	auto temp_base = m_subject.Map().Find("c:@ST>1#T@RecursiveBase");
	
	ASSERT_TRUE(temp_base);
	ASSERT_EQ("RecursiveBase<BaseType>", temp_base->Name());
	
	auto inst_base = m_subject.Map().TypedFind<DataType>("c:@S@RecursiveBase>#$@S@Base");
	ASSERT_TRUE(inst_base);
	
	std::vector<TypeRef> bbase{
		TypeRef{"c:@S@Base"},
		TypeRef{"c:@S@Base4"}
	};
	ASSERT_EQ(bbase, inst_base->BaseClasses());
}

TEST_F(TemplateBaseClassTest, Test_template_fields)
{
	auto base = dynamic_cast<const DataType*>(m_subject.Map().FindByName("Base"));
	auto base_fields = base->Fields();
	
	ASSERT_EQ(2, base_fields.size());
	auto& base_field = base_fields[0];
	
	auto field_type = m_subject.Map().TypedFind<DataType>(base_field.TypeRef().ID());
	ASSERT_TRUE(field_type);
	ASSERT_EQ("Temp1<int>", field_type->Name());
	
	auto temp1_fields = field_type->Fields();
	ASSERT_EQ(1, temp1_fields.size());
	ASSERT_EQ(CXType_Int, temp1_fields.front().TypeRef().Kind());
}
