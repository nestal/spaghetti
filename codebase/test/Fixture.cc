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

#include <boost/filesystem/path.hpp>

namespace codebase {
namespace ut {

namespace fs = boost::filesystem;

void Fixture::SetUp()
{
	m_subject.Parse((fs::path{__FILE__}.parent_path() / "testdata" / m_source).string(),
		{
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", SRC_DIR
		});
}


}} // end of namespace
