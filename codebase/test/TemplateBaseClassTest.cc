/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/11/17.
//

#include "codebase/CodeBase.hh"
#include "codebase/DataType.hh"

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>

using namespace codebase;
namespace fs = boost::filesystem;

class TemplateBaseClassTest : public testing::Test
{
protected:
	void SetUp() override
	{
		m_subject.Parse((fs::path{__FILE__}.parent_path()/"testdata/test.cc").string(), {
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", SRC_DIR
		});
	}
	
	CodeBase m_subject;
};

TEST_F(TemplateBaseClassTest, Test_base_class)
{
	auto derived = dynamic_cast<const DataType*>(m_subject.Map().FindByName("Derived"));
	ASSERT_EQ("Derived", derived->Name());
	
	std::vector<std::string> base{"c:@S@RecursiveBase>#$@S@Base", "c:@S@Base2", "c:@S@Base3"};
	ASSERT_EQ(base, derived->BaseClasses());
	
	auto temp_base = m_subject.Map().Find("c:@S@RecursiveBase>#$@S@Base");
	ASSERT_TRUE(temp_base);
}
