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

#include "codebase/EntityVec.hh"
#include "MockEntity.hh"

using namespace codebase;
using namespace codebase::ut;

class TestEntityVec : public EntityVec
{
public:
	using EntityVec::EntityVec;
	void RemoveUnused() override {}
	std::string Type() const override {return {};}
};

TEST(EntityVecTest, Constructor_Wont_Throw)
{
	TestEntityVec subject{"", "", nullptr};
}

TEST(EntityVecTest, Add_Return_Iterator_To_Added_Item)
{
	TestEntityVec subject{"", "vec-ID", nullptr};
	auto it = subject.Add<MockEntity>(subject.ChildCount(), &subject);
	
	ASSERT_EQ(&subject, it->Parent());
}
