/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/13/17.
//

#include "codebase/CodeBase.hh"
#include "codebase/DataType.hh"
#include "codebase/EntityMap.hh"

#include <gtest/gtest.h>

using namespace codebase;

TEST(CodeBaseTest, TestInheritance)
{
	CodeBase subject;
	subject.Parse(SRC_DIR "/codebase/CodeBase.cc", {
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", SRC_DIR
	});
	
	auto entityvec_class = dynamic_cast<const DataType*>(subject.Map().Find("c:@N@codebase@S@EntityVec"));
	auto entity_class   = dynamic_cast<const Entity*>(subject.Map().Find("c:@N@codebase@S@Entity"));
	
	ASSERT_TRUE(entityvec_class);
	ASSERT_TRUE(entity_class);
	
	ASSERT_EQ(TypeRef{"c:@N@codebase@S@EntityVec"}.SetName("EntityVec"), entityvec_class->GetTypeRef());
	
	for (auto&& base : entityvec_class->BaseClasses())
		ASSERT_EQ(entity_class->ID(), base.ID());
}
