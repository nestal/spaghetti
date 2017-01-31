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
	if (!loc.IsFromSystemHeader())
	{
		switch (cursor.Kind())
		{
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl:
		{
			auto it = m_classes.find(cursor.USR());
			if (it == m_classes.end())
				it = m_classes.emplace(cursor).first;

			Class::Data data;
			it->Visit(data, cursor);
			
			m_classes.modify(it, [&data](Class& c){c.Merge(std::move(data));});
			break;
		}
			
		case CXCursor_Namespace:
		{
			std::cout << "namespace: " << cursor.Spelling() << "\n";
			cursor.Visit([this](clx::Cursor cursor, clx::Cursor parent)
			{
				Visit(cursor, parent);
			});
			std::cout << "end namespace: " << cursor.Spelling() << "\n";
			break;
		}
		
		default:
			std::cout << "default: " << cursor.Spelling() << " " << cursor.Kind() << "\n";
			break;
		}
	}
}

void CodeBase::Parse(const std::string& source)
{
	auto tu = m_index.Parse(
		source.c_str(),
		{
			"-std=c++14",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
			"-I", ".",
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

CodeBase::usr_iterator CodeBase::find(const std::string& usr) const
{
	return m_classes.get<ByUSR>().find(usr);
}

CodeBase::usr_iterator CodeBase::begin() const
{
	return m_classes.get<ByUSR>().begin();
}

CodeBase::usr_iterator CodeBase::end() const
{
	return m_classes.get<ByUSR>().end();
}
	
} // end of namespace
