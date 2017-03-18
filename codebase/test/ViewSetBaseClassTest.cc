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
		
		m_viewset = dynamic_cast<const DataType*>(m_subject.Map().FindByName("ViewSet"));
	}
	
	CodeBase m_subject;
	const DataType *m_viewset;
};

TEST_F(ViewSetBaseClassTest, Test_ViewSet_Base)
{
	ASSERT_TRUE(m_viewset);
	ASSERT_EQ(std::vector<ClassRef>{
		{"c:@S@QTabWidget"}
	}, m_viewset->BaseClasses());
	
	auto base = m_subject.Map().Find(m_viewset->BaseClasses().front().BaseID());
	ASSERT_TRUE(base);
	ASSERT_EQ("QTabWidget", base->Name());
}

TEST_F(ViewSetBaseClassTest, Test_Nested_iterator_Classes)
{
	auto it = std::find_if(m_viewset->begin(), m_viewset->end(), [](auto&& entity){return entity.Name() == "iterator";});
	ASSERT_NE(m_viewset->end(), it);
	ASSERT_EQ(m_viewset, it->Parent());
}