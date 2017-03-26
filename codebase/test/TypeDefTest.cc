/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

#include "Fixture.hh"

using namespace codebase;

class TypeDefTest : public ut::Fixture
{
protected:
	TypeDefTest() : Fixture{"typedef.cc"}
	{
		
	}
};

TEST_F(TypeDefTest, Test_typedef)
{
	
}
