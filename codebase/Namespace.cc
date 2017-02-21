/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "Namespace.hh"

namespace codebase {

Namespace::Namespace() :
	m_name{"Root"},
	m_usr{},
	m_parent{NullID()},
	m_ns{"Namespaces", m_usr},
	m_types{"Types", m_usr},
	m_vars{"Variables", m_usr}
{
}

Namespace::Namespace(libclx::Cursor cursor, const std::string& parent) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_parent{parent},
	m_ns{"Namespaces", m_usr},
	m_types{"Types", m_usr},
	m_vars{"Variables", m_usr}
{
}

const std::string& Namespace::Name() const
{
	return m_name;
}

const std::string& Namespace::ID() const
{
	return m_usr;
}

const std::string& Namespace::Parent() const
{
	return m_parent;
}

std::string Namespace::Type() const
{
	return "Namespace";
}

std::size_t Namespace::ChildCount() const
{
	return 3;
}

const Entity *Namespace::Child(std::size_t idx) const
{
	switch (idx)
	{
	case 0: return &m_types;
	case 1: return &m_vars;
	case 2: return &m_ns;
	default: return nullptr;
	}
}

Entity *Namespace::Child(std::size_t idx)
{
	switch (idx)
	{
	case 0: return &m_types;
	case 1: return &m_vars;
	case 2: return &m_ns;
	default: return nullptr;
	}
}

std::size_t Namespace::IndexOf(const Entity *child) const
{
	return child == &m_types ? 0 :
		(child == &m_vars ? 1 :
		(child == &m_ns ? 2 : npos));
}

void Namespace::Visit(libclx::Cursor self)
{
	self.Visit([this](libclx::Cursor cursor, libclx::Cursor)
	{
		if (cursor.Location().IsFromSystemHeader())
			return;
		
		const std::string& id = cursor.USR();
		switch (cursor.Kind())
		{
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl:
		{
			auto it = std::find_if(
				m_types.begin(), m_types.end(), [id](auto& t)
				{
					return t.ID() == id;
				}
			);
			if (it == m_types.end())
				it = m_types.Add(cursor, m_types.ID());
			it->Visit(cursor);
			break;
		}
		
		case CXCursor_Namespace:
		{
			auto it = std::find_if(
				m_ns.begin(), m_ns.end(), [id](auto& t)
				{
					return t.ID() == id;
				}
			);
			if (it == m_ns.end())
				it = m_ns.Add(cursor, m_ns.ID());
			it->Visit(cursor);
			break;
		}
		
		case CXCursor_FieldDecl:
		{
			m_vars.Add(cursor, m_vars.ID());
			break;
		}
		
		default: break;
		}
	});
}
	
} // end of namespace
