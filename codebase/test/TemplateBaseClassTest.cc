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

int global_var = 0;

class Base
{
public:
	virtual ~Base() = default;
	virtual void Func() = 0;
};

template <typename BaseType>
class RecursiveBase : public BaseType
{
public:
	virtual void SomeFunc()
	{
		// prevent optimizer to remove the function
		global_var++;
	}
};

class Base2
{
public:
	Base2() = default;
};

class Derived : public RecursiveBase<Base>, public Base2
{
public:
	Derived() = default;
	
	void Func() override
	{
		SomeFunc();
	}
};

using namespace codebase;
namespace fs = boost::filesystem;

TEST(TemplateBaseClassTest, Test_base_class)
{
	CodeBase subject;
	subject.Parse(__FILE__, {
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"-I", "."
	});
	
	auto derived = dynamic_cast<const DataType*>(subject.Map().FindByName("Derived"));
	ASSERT_EQ("Derived", derived->Name());
//	ASSERT_EQ(1, derived->BaseClasses().size());
	
	for (auto&& base_id : derived->BaseClasses())
	{
		auto base_entity = subject.Map().Find(base_id);
		ASSERT_TRUE(base_entity);
		
		ASSERT_EQ("RecursiveBase", base_entity->Name());
	}
}