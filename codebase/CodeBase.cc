/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "CodeBase.hh"
#include "EditAction.hh"

#include <iostream>

namespace codebase {

CodeBase::CodeBase()
{
	AddToIndex(this);
}

void CodeBase::Visit(libclx::Cursor cursor, libclx::Cursor)
{
	switch (cursor.Kind())
	{
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
	{
		auto it = std::find_if(m_types.begin(), m_types.end(), [usr=cursor.USR()](auto& t)
		{
			return t.ID() == usr;
		});
		if (it == m_types.end())
			it = m_types.Add(DataType{cursor, m_types.ID()});
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
	
	m_search_index.clear();
	tu.Root().Visit([this](libclx::Cursor cursor, libclx::Cursor parent)
	{
		Visit(cursor, parent);
	});
	AddToIndex(this);
	
	for (auto&& diag : tu.Diagnostics())
		std::cerr << diag.Str() << "\n";
	
	m_units.push_back(std::move(tu));
	
	return tu.Spelling();
}

const std::string& CodeBase::Name() const
{
	static std::string name{"codebase"};
	return name;
}

const std::string& CodeBase::Parent() const
{
	return NullID();
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

std::size_t CodeBase::IndexOf(const Entity *entity) const
{
	return entity == &m_types ? 0 : ChildCount();
}

std::string CodeBase::Type() const
{
	return "Code base";
}

const std::string& CodeBase::ID() const
{
	return NullID();
}

const Entity *CodeBase::Find(const std::string& id) const
{
	auto it = m_search_index.find(id);
	return it != m_search_index.end() ? *it : nullptr;
}

void CodeBase::AddToIndex(const Entity *entity)
{
	m_search_index.insert(entity);
	
	for (auto&& c : *entity)
		AddToIndex(&c);
}
	
} // end of namespace
