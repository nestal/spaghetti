/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

#include "Fixture.hh"

#include "codebase/EntityMap.hh"
#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/TypeAlias.hh"

using namespace codebase;

class TypeAliasTest : public ut::Fixture
{
protected:
	TypeAliasTest() : Fixture{"typedef.cc"}
	{
	}
	
	void SetUp() override
	{
		ut::Fixture::SetUp();
		
		m_owner = dynamic_cast<const DataType*>(m_map.FindByName("TypedefOwner"));
		ASSERT_TRUE(m_owner);
		
		auto fields = m_owner->Fields();
		ASSERT_EQ(3, fields.size());
		m_str   = &fields[0];
		m_ptstr = &fields[1];
		m_pstr  = &fields[2];
	}
	
	const DataType *m_owner{};
	const Variable *m_str{};
	const Variable *m_ptstr{};
	const Variable *m_pstr{};
};

TEST_F(TypeAliasTest, Test_typedef)
{
	ASSERT_TRUE(m_str);
	auto& type = m_str->TypeRef();
	
	ASSERT_EQ("m_str",        m_str->Name());
	ASSERT_EQ("Str",          type.Name());
	ASSERT_EQ(CXType_Typedef, type.Kind());
	
	auto string_alias = m_map.TypedFind<TypeAlias>(type.ID());
	ASSERT_TRUE(string_alias);
	
	auto real_type = string_alias->Dest();
	ASSERT_EQ("Temp<String>",   real_type.Name());
	ASSERT_EQ("c:@ST>1#T@Temp", real_type.TemplateID());
	ASSERT_EQ(1, real_type.TempArgs().size());
	ASSERT_EQ("String", real_type.TempArgs().front().Name());
	
	auto class_type = m_str->ClassType();
	ASSERT_TRUE(class_type);
	ASSERT_EQ("Temp<String>", class_type->Name());
	ASSERT_TRUE(class_type->IsUsedInMember(*m_owner));
}

TEST_F(TypeAliasTest, Test_pointer_field)
{
	ASSERT_TRUE(m_pstr);
	auto& type = m_pstr->TypeRef();
	
	ASSERT_EQ("m_pstr", m_pstr->Name());
	ASSERT_EQ("String",       type.Name());
	ASSERT_EQ(CXType_Pointer, type.Kind());
	ASSERT_EQ("c:@S@String",  type.ID());
	
	auto string_class = dynamic_cast<const DataType*>(m_map.FindByName("String"));
	ASSERT_TRUE(string_class);
	ASSERT_EQ(string_class->ID(), type.ID());
	ASSERT_EQ(string_class, m_pstr->ClassType());
}

TEST_F(TypeAliasTest, Test_template_pointer_field)
{
	ASSERT_TRUE(m_ptstr);
	auto& type = m_ptstr->TypeRef();
	
	ASSERT_EQ("m_ptstr",      m_ptstr->Name());
	ASSERT_EQ("Str",          type.Name());
	ASSERT_EQ(CXType_Pointer, type.Kind());
	
	auto string_alias = m_map.TypedFind<TypeAlias>(type.ID());
	ASSERT_TRUE(string_alias);
	
	auto real_type = string_alias->Dest();
	ASSERT_EQ("Temp<String>",   real_type.Name());
	ASSERT_EQ("c:@ST>1#T@Temp", real_type.TemplateID());
	ASSERT_EQ(1, real_type.TempArgs().size());
	ASSERT_EQ("String", real_type.TempArgs().front().Name());
}
