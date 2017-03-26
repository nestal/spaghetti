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

#include "codebase/CodeBase.hh"

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/EntityMap.hh"

#include <boost/filesystem/path.hpp>

using namespace codebase;
namespace fs = boost::filesystem;

class UniquePtrTest : public testing::Test
{
protected:
	void SetUp() override
	{
		m_subject.Parse((fs::path{__FILE__}.parent_path()/"testdata/unique_test.cc").string(), {
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", SRC_DIR
		});
	}
	
	CodeBase m_subject;
};

TEST_F(UniquePtrTest, Test_unique_ptr_to_int)
{
	auto derived = dynamic_cast<const DataType *>(m_subject.Map().FindByName("Test"));
	ASSERT_EQ("Test", derived->Name());
}
