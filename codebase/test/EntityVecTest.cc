/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include <gtest/gtest.h>

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/Function.hh"

#include "codebase/EntityVec.hh"
#include "codebase/EntityType.hh"

#include "util/MultiContainer.hh"

#include "libclx/Cursor.hh"

#include "MockEntity.hh"

using namespace codebase;
using namespace codebase::ut;

class TestEntityVec : public EntityVec
{
public:
	using EntityVec::EntityVec;
	EntityType Type() const override {return EntityType::none;}
	void CrossReference(EntityMap *) override {}
};

TEST(EntityVecTest, Constructor_Wont_Throw)
{
	TestEntityVec subject;
	ASSERT_EQ(Entity::NullID(), subject.ID());
}

TEST(EntityVecTest, Add_Return_Iterator_To_Added_Item)
{
	TestEntityVec subject;
	auto it = subject.Add<MockEntity>(subject.ChildCount(), &subject);
	
	ASSERT_EQ(&subject, it->Parent());
	ASSERT_EQ(1, subject.ChildCount());
	ASSERT_EQ(0, subject.IndexOf(&*it));
}

TEST(EntityVecTest, Add_Unique_With_a_Vector_Disallow_Duplicates)
{
	TestEntityVec subject;
	std::vector<MockEntity*> vec;
	
	auto m0 = subject.AddUnique(vec, "mock0ID", 0, &subject);
	ASSERT_EQ("mock0ID", m0->ID());
	ASSERT_EQ(1, subject.ChildCount());
	
	auto m1 = subject.AddUnique(vec, "mock0ID", 0, &subject);
	ASSERT_EQ(m0, m1);
	ASSERT_EQ(1, subject.ChildCount());
}

TEST(EntityVecTest, Test_Variadic_Test)
{
	util::MultiContainer<Entity, MockDataType, Variable, Function> vec;
	
	TestEntityVec subject;
	
	// prevent reallocation
	util::Get<MockDataType>(vec).reserve(2);
		
	auto& dt1  = Add(vec, MockDataType{&subject});
	auto& dt2  = Add(vec, MockDataType{&subject});
	auto& var1 = Add(vec, Variable{libclx::Cursor{}, &subject});
	auto& fn1  = Add(vec, Function{libclx::Cursor{}, &subject});
	
	ASSERT_EQ(4, vec.Size());
	
	auto& vars = util::Get<Variable>(vec);
	ASSERT_EQ(1, vars.size()) ;
	ASSERT_EQ(&var1, &vars.front());
	
	auto& dts = util::Get<MockDataType>(vec);
	ASSERT_EQ(2, dts.size()) ;
	ASSERT_EQ(&dt1, &dts.front());
	ASSERT_EQ(&dt2, &dts.back());
	
	ASSERT_EQ(0, vec.IndexOf(vec.At(0)));
	ASSERT_EQ(1, vec.IndexOf(vec.At(1)));
	ASSERT_EQ(2, vec.IndexOf(vec.At(2)));
}
