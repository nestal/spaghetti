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
	EntityVec{"Root", "", nullptr}
{
}

Namespace::Namespace(libclx::Cursor cursor, const Entity* parent) :
	EntityVec{cursor.Spelling(), cursor.USR(), parent}
{
}

std::string Namespace::Type() const
{
	return "Namespace";
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
			auto it = std::find_if(m_types.begin(), m_types.end(), [id](auto& t){return t->ID() == id;});
			if (it == m_types.end())
			{
				m_types.push_back(Add<DataType>(cursor, this));
				it = --m_types.end();
			}
			(*it)->Visit(cursor);
			break;
		}
		
		case CXCursor_Namespace:
		{
			auto it = std::find_if(m_ns.begin(), m_ns.end(), [id](auto& t){return t->ID() == id;});
			if (it == m_ns.end())
			{
				m_ns.push_back(Add<Namespace>(cursor, this));
				it = --m_ns.end();
			}
			(*it)->Visit(cursor);
			break;
		}
		
		case CXCursor_FieldDecl:
		{
			m_vars.push_back(Add<Variable>(cursor, this));
			break;
		}
		
		default: break;
		}
	});
}

void Namespace::RemoveUnused()
{
	
}

} // end of namespace
