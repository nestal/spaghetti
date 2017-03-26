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
};

TEST_F(TypeAliasTest, Test_typedef)
{
	auto owner_class = dynamic_cast<const DataType*>(m_map.FindByName("TypedefOwner"));
	ASSERT_TRUE(owner_class);
	
	auto fields = owner_class->Fields();
	ASSERT_EQ(2, fields.size());
	
	auto& str_field = fields.front();
	ASSERT_EQ("m_str", str_field.Name());
	ASSERT_EQ("Str", str_field.TypeRef().Name());
	ASSERT_EQ(CXType_Typedef, str_field.TypeRef().Kind());
	
	auto string_alias = m_map.TypedFind<TypeAlias>(str_field.TypeRef().ID());
	ASSERT_TRUE(string_alias);
	
	auto real_type = string_alias->Dest();
	ASSERT_EQ("Temp<String>", real_type.Name());
	ASSERT_EQ("c:@ST>1#T@Temp", real_type.TemplateID());
	ASSERT_EQ(1, real_type.TempArgs().size());
	ASSERT_EQ("String", real_type.TempArgs().front().Name());
	
	auto class_type = str_field.ClassType();
	ASSERT_TRUE(class_type);
	ASSERT_EQ("Temp<String>", class_type->Name());
	ASSERT_TRUE(class_type->IsUsedInMember(*owner_class));
}

TEST_F(TypeAliasTest, Test_pointer_field)
{
	auto owner_class = dynamic_cast<const DataType*>(m_map.FindByName("TypedefOwner"));
	ASSERT_TRUE(owner_class);
	
	auto fields = owner_class->Fields();
	ASSERT_EQ(2, fields.size());
	auto& str_field = fields[1];
	auto& field_type = str_field.TypeRef();
	
	ASSERT_EQ("m_pstr", str_field.Name());
	ASSERT_EQ(CXType_Pointer, field_type.Kind());
	ASSERT_EQ("c:@S@String",  field_type.ID());
	
	auto string_class = dynamic_cast<const DataType*>(m_map.FindByName("String"));
	ASSERT_TRUE(string_class);
	ASSERT_EQ(string_class->ID(), field_type.ID());
	ASSERT_EQ(string_class, str_field.ClassType());
	
	ASSERT_EQ("String", field_type.Name());
}
