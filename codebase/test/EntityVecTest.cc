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
	TestEntityVec() = default;
	using EntityVec::EntityVec;
	TestEntityVec(TestEntityVec&& r) : EntityVec{std::move(r)}, m_cond{std::move(r.m_cond)}
	{
		for (auto&& child : *this)
			child.Reparent(this);
	}
	
	EntityType Type() const override {return EntityType::none;}
	void CrossReference(EntityMap *) override {}
	
	template <typename Type, typename... Ts>
	Type& Add(const std::string& id, Ts... ts)
	{
		return *AddUnique(m_cond, id, ts...);
	}

private:
	std::vector<MockEntity*> m_cond;
};

TEST(EntityVecTest, Constructor_Wont_Throw)
{
	TestEntityVec subject;
	ASSERT_EQ(Entity::NullID(), subject.ID());
}

TEST(EntityVecTest, Add_Return_Iterator_To_Added_Item)
{
	TestEntityVec subject;
	auto& ref = subject.Add<MockEntity>("id1", subject.ChildCount(), &subject);
	
	ASSERT_EQ(&subject, ref.Parent());
	ASSERT_EQ(1, subject.ChildCount());
	ASSERT_EQ(0, subject.IndexOf(&ref));
}

TEST(EntityVecTest, Add_Unique_With_a_Vector_Disallow_Duplicates)
{
	TestEntityVec subject;
	
	auto& m0 = subject.Add<MockEntity>("mock0ID", 0, &subject);
	ASSERT_EQ("mock0ID", m0.ID());
	ASSERT_EQ(1, subject.ChildCount());
	
	auto& m1 = subject.Add<MockEntity>("mock0ID", 0, &subject);
	ASSERT_EQ(&m0, &m1);
	ASSERT_EQ(1, subject.ChildCount());
}

TEST(EntityVecTest, Move_Ctor_Reparent_All_Children)
{
	TestEntityVec subject;
	
	subject.Add<MockEntity>("mock0ID", 0, &subject);
	subject.Add<MockEntity>("mock1ID", 0, &subject);
	subject.Add<MockEntity>("mock2ID", 0, &subject);
	subject.Add<MockEntity>("mock3ID", 0, &subject);
	ASSERT_EQ(4, subject.ChildCount());
	
	TestEntityVec moved{std::move(subject)};
	ASSERT_EQ(4, moved.ChildCount());
}
