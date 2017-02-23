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
	EntityVec{"Root", "", NullID()}
{
}

Namespace::Namespace(libclx::Cursor cursor, const std::string& parent) :
	EntityVec{cursor.Spelling(), cursor.USR(), parent}
{
}

std::string Namespace::Type() const
{
	return "Namespace";
}

void Namespace::Visit(libclx::Cursor self)
{
	std::vector<DataType>  types;
	std::vector<Namespace> ns;
	std::vector<Variable>  vars;
	
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
			{
				types.emplace_back(cursor, ID());
				it = --types.end();
			}
			it->Visit(cursor);
			break;
		}
		
		case CXCursor_Namespace:
		{
			auto it = std::find_if(ns.begin(), ns.end(), [id, &ns](auto& t){return t.ID() == id;});
			if (it == ns.end())
			{
				ns.emplace_back(cursor, ID());
				it = --ns.end();
			}
			it->Visit(cursor);
			break;
		}
		
		case CXCursor_FieldDecl:
		{
			vars.emplace_back(cursor, ID());
			break;
		}
		
		default: break;
		}
	});
	
	// after the vectors are built, we can take the address of their contents
	for (auto&& type : types)
		Add(std::make_unique<DataType>(std::move(type)));
	for (auto&& ns : ns)
		Add(std::make_unique<Namespace>(std::move(ns)));
	for (auto&& var : vars)
		Add(std::make_unique<Variable>(std::move(var)));
}
	
} // end of namespace
