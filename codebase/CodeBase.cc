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
#include "EditAction.hh"

#include <iostream>

namespace codebase {

void CodeBase::Visit(libclx::Cursor cursor, libclx::Cursor)
{
	auto loc = cursor.Location();
	if (!loc.IsFromSystemHeader())
	{
		switch (cursor.Kind())
		{
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl:
		{
//			auto& usr = m_classes.get<ByUSR>();
			auto it = std::find_if(m_types.begin(), m_types.end(), [usr=cursor.USR()](auto& t)
			{
				return t.USR() == usr;
			});
			if (it == m_types.end())
				it = m_types.Add(DataType(cursor, &m_types));
			
			EditAction data;
			it->Visit(data, cursor);
			it->Merge(std::move(data));
			
			//usr.modify(it, [&data](DataType& c){c.Merge(std::move(data));});
			break;
		}
			
		case CXCursor_Namespace:
		{
			cursor.Visit([this](libclx::Cursor cursor, libclx::Cursor parent)
			{
				Visit(cursor, parent);
			});
			break;
		}
		
		default:
			break;
		}
	}
}

std::string CodeBase::Parse(const std::string& source)
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
	
	tu.Root().Visit([this](libclx::Cursor cursor, libclx::Cursor parent)
	{
		Visit(cursor, parent);
	});
	
	for (auto&& diag : tu.Diagnostics())
		std::cerr << diag.Str() << "\n";
	
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

const DataType* CodeBase::Find(const SourceLocation& /*loc*/) const
{
//	auto it = m_classes.get<ByLocation>().find(loc);
	return /*it != m_classes.get<ByLocation>().end() ? &*it : */nullptr;
}

void CodeBase::Add(const DataType *, const SourceLocation& )
{
//	m_classes.get<ByLocation>().emplace(type);
}

const Entity *CodeBase::Root() const
{
	return &m_types;
}

} // end of namespace
