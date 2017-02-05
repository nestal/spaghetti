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

TEST(EntityVecTest, Constructor_Wont_Throw)
{
	EntityVec<MockEntity> subject{"", ""};
}

TEST(EntityVecTest, Add_Return_Iterator_To_Added_Item)
{
	EntityVec<MockEntity> subject{"", "vec-ID"};
	auto it = subject.Add(MockEntity{subject.ChildCount(), subject.ID()});
	
	ASSERT_EQ(subject.ID(), it->Parent());
}
