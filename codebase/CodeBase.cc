/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "CodeBase.hh"

#include <iostream>

namespace cb {

void CodeBase::Visit(clx::Cursor cursor, clx::Cursor parent)
{
	auto loc = cursor.Location();
	if (loc.IsFromMainFile())
		std::cout << '\"' << cursor.DisplayName() << "\", \"" << cursor.Spelling() << "\", " << cursor.Kind() << "\n";
	
}

void CodeBase::Parse(const std::string& source)
{
	auto tu = m_index.Parse(
		source.c_str(),
		{
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/"
		},
		CXTranslationUnit_None
	);
	
	std::cout << "translated " << tu.Spelling() << "\n";
	tu.Root().Visit([this](clx::Cursor cursor, clx::Cursor parent)
	{
		Visit(cursor, parent);
	});
	
	for (unsigned i = 0, n = clang_getNumDiagnostics(tu.Get()); i != n; ++i)
	{
		auto diag = ::clang_getDiagnostic(tu.Get(), i);
		auto str = ::clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
		std::cerr << ::clang_getCString(str) << "\n";
		::clang_disposeString(str);
	}

	m_units.push_back(std::move(tu));
}
	
} // end of namespace
