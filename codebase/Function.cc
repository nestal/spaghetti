/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "Function.hh"
#include "Variable.hh"
#include "EntityType.hh"

#include "libclx/Cursor.hh"
#include "libclx/Type.hh"

#include <iostream>
#include <sstream>

namespace codebase {

Function::Function(const libclx::Cursor& first_seen, const EntityVec *parent) :
	EntityVec{first_seen.Spelling(), first_seen.USR(), parent},
	m_definition{first_seen.Location()},
	m_return_type{first_seen.ResultType()}
{
}

libclx::SourceLocation Function::Location() const
{
	return m_definition;
}

EntityType Function::Type() const
{
	return EntityType::function;
}

void Function::Visit(const libclx::Cursor& self)
{
	assert(Parent());
	m_definition = self.Location();
	
	self.Visit([this](libclx::Cursor cursor, libclx::Cursor)
	{
		switch (cursor.Kind())
		{
		case CXCursor_ParmDecl:
			AddUnique(m_args, cursor.USR(), cursor, this);
			break;
			
		default:
//			if (!cursor.Location().IsFromSystemHeader())
//				std::cout << Name() << " " <<  cursor.Spelling() << ' ' << cursor.Kind() << std::endl;
			break;
		}
	});
	
	// mark self and all children as used, after creating the children
	if (self.IsDefinition() && self.Location().IsFromMainFile())
		SetUsed();
}

void Function::CrossReference(EntityMap *)
{
}

std::string Function::UML() const
{
	std::ostringstream oss;
	oss << Name() << "(";
//	for (auto& arg : m_args)
//		oss << arg << ", ";
	oss << ") : " << m_return_type.Spelling();
	return oss.str();
}

libclx::Type Function::ReturnType() const
{
	return m_return_type;
}

} // end of namespace
