/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

#pragma once

#include <gtest/gtest.h>

#include "codebase/CodeBase.hh"

namespace codebase {
namespace ut {

class Fixture : public testing::Test
{
protected:
	Fixture(const std::string& source) : m_source{source}
	{
	}
	
	void SetUp() override;
	
	std::string m_source;
	
	CodeBase m_subject;
	EntityMap& m_map{m_subject.Map()};
};

}} // end of namespace
