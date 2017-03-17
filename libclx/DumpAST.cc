/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/17/17.
//

#include "Index.hh"
#include "Cursor.hh"
#include "Type.hh"

#include <iostream>

using namespace libclx;

int main(int argc, char **argv)
{
	Index idx;
	for (auto i = 1; i < argc; i++)
	{
		auto tu = idx.Parse(argv[i], {
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", "."
		}, CXTranslationUnit_SkipFunctionBodies|CXTranslationUnit_DetailedPreprocessingRecord);
		
		tu.Root().Visit([](Cursor c, Cursor)
		{
			std::cout << c.KindSpelling() << " (" << c.Spelling() << ", " << c.USR() << ")";
			if (c.IsReference())
			{
				auto&& r = c.Referenced();
				std::cout << " -> " << r.KindSpelling() << " (" << r.Spelling() << ", " << r.USR() << ")";
				
				if (r.USR() == "c:@S@RecursiveBase>#$@S@Base")
				{
					std::cout << " " << r.Type();
					auto&& def = r.GetDefinition();
					
					def.Visit([](Cursor base, Cursor)
					{
						std::cout << "RB: " << base.Spelling() << " " << base.KindSpelling() << std::endl;
					});
				}
			}
			
			std::cout << std::endl;
		}, true);
	}
	return 0;
}
