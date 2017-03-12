/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/12/17.
//

#include "codebase/CodeBase.hh"
#include "codebase/DataType.hh"

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>

using namespace codebase;
namespace fs = boost::filesystem;

class ViewSetBaseClassTest : public testing::Test
{
protected:
	void SetUp() override
	{
		m_subject.Parse((fs::path{SRC_DIR}/"gui/ViewSet.cc").string(), {
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", SRC_DIR,
			"-I", "/usr/include/qt5",
			"-I", "/usr/include/qt5/QtCore",
			"-I", "/usr/include/qt5/QtGui",
			"-I", "/usr/include/qt5/QtWidgets",
			"-fPIC"
		});
		
		for (auto&& tu : m_subject.TranslationUnits())
		{
			for (auto&& diag : tu.Diagnostics())
				std::cerr << tu.Spelling() << ": " << diag.Str() << std::endl;
		}
	}
	
	CodeBase m_subject;
};

TEST_F(ViewSetBaseClassTest, Test_ViewSet_Base)
{
	auto viewset = dynamic_cast<const DataType*>(m_subject.Map().FindByName("ViewSet"));
	ASSERT_TRUE(viewset);
	ASSERT_EQ(1, viewset->BaseClasses().size());
	
	auto base_id = viewset->BaseClasses().front();
	ASSERT_EQ("c:@S@QTabWidget", base_id);
	
	auto base = m_subject.Map().Find(base_id);
	ASSERT_TRUE(base);
	ASSERT_EQ("QTabWidget", base->Name());
}