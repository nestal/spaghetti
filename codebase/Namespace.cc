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
	m_parent{NullID()}
{
}

Namespace::Namespace(libclx::Cursor cursor, const std::string& parent) :
	m_name{cursor.Spelling()},
	m_usr{cursor.USR()},
	m_parent{parent}
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
	return m_children.size();
}

const Entity *Namespace::Child(std::size_t idx) const
{
	return m_children.at(idx).get();
}

Entity *Namespace::Child(std::size_t idx)
{
	return m_children.at(idx).get();
}

std::size_t Namespace::IndexOf(const Entity *child) const
{
	auto it = std::find_if(m_children.begin(), m_children.end(), [child](auto& c){return c.get() == child;});
	return it != m_children.end() ? static_cast<std::size_t>(it - m_children.begin()) : npos;
}

void Namespace::Visit(libclx::Cursor self)
{
	EntityVec<DataType>  types{"", ID()};
	EntityVec<Namespace> ns{"", ID()};
	EntityVec<Variable>  vars{"", ID()};
	
	self.Visit([this, &types, &ns, &vars](libclx::Cursor cursor, libclx::Cursor)
	{
		if (cursor.Location().IsFromSystemHeader())
			return;
		
		const std::string& id = cursor.USR();
		switch (cursor.Kind())
		{
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl:
		{
			auto it = std::find_if(types.begin(), types.end(), [id, &types](auto& t){return t.ID() == id;});
			if (it == types.end())
				it = types.Add(cursor, ID());
			it->Visit(cursor);
			break;
		}
		
		case CXCursor_Namespace:
		{
			auto it = std::find_if(ns.begin(), ns.end(), [id, &ns](auto& t){return t.ID() == id;});
			if (it == ns.end())
				it = ns.Add(cursor, ID());
			it->Visit(cursor);
			break;
		}
		
		case CXCursor_FieldDecl:
		{
			vars.Add(cursor, ID());
			break;
		}
		
		default: break;
		}
	});
	
	// after the vectors are built, we can take the address of their contents
	for (auto&& type : types)
		m_children.push_back(std::make_unique<DataType>(std::move(type)));
	for (auto&& ns : ns)
		m_children.push_back(std::make_unique<Namespace>(std::move(ns)));
	for (auto&& var : vars)
		m_children.push_back(std::make_unique<Variable>(std::move(var)));
}
	
} // end of namespace
