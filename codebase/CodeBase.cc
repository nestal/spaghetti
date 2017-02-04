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
			auto it = std::find_if(m_types.begin(), m_types.end(), [usr=cursor.USR()](auto& t)
			{
				return t.USR() == usr;
			});
			if (it == m_types.end())
				it = m_types.Add(DataType(cursor));
			it->Visit(cursor);
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
	
	Reparent(nullptr);
	
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

const std::string& CodeBase::Name() const
{
	static std::string name{"codebase"};
	return name;
}

const Entity* CodeBase::Parent() const
{
	return nullptr;
}

std::size_t CodeBase::ChildCount() const
{
	return 1;
}

const Entity* CodeBase::Child(std::size_t idx) const
{
	return idx == 0 ? &m_types : nullptr;
}

Entity *CodeBase::Child(std::size_t idx)
{
	return idx == 0 ? &m_types : nullptr;
}

std::size_t CodeBase::IndexOf(const Entity *) const
{
	return 0;
}

std::string CodeBase::Type() const
{
	return "Code base";
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

void CodeBase::OnReparent(const Entity *)
{
}

CodeBase::CodeBase()
{
	m_types.Reparent(this);
}
	
} // end of namespace
