/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#include "TypeRef.hh"
#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"
#include "libclx/Type.hh"

#include <ostream>
#include <cassert>

namespace codebase {

TypeRef::TypeRef(const libclx::Type& type)
{
	FromType(type);
}

TypeRef::TypeRef(const libclx::Cursor& cursor) :
	m_base_id{cursor.USR()},
	m_name{cursor.DisplayName()},
	m_type_kind{cursor.Type().Kind()}
{
//	std::cout << "created TypeRef for " << cursor.KindSpelling() << " base(" << m_base_id << ") temp(" << m_temp_id << ") " << m_name << std::endl;
	assert(
		cursor.Kind() == CXCursor_CXXBaseSpecifier ||
		cursor.Kind() == CXCursor_FieldDecl ||
		cursor.Kind() == CXCursor_ParmDecl
	);
	
	cursor.Visit([this](libclx::Cursor child, libclx::Cursor parent)
	{
		switch (child.Kind())
		{
		case CXCursor_TemplateRef:
			FromTemplateRef(child, parent.Type());
			break;
		
		case CXCursor_TypeRef:
			if (m_temp_id.empty())
				m_base_id = child.Referenced().USR();
			break;
		
		default:
			break;
		}
	});

//	std::cout << "created TypeRef for " << cursor.KindSpelling() << " base(" << m_base_id << ") temp(" << m_temp_id << ") " << m_name << std::endl;
}

const std::string& TypeRef::ID() const
{
	return m_base_id;
}

const std::vector<TypeRef>& TypeRef::TempArgs() const
{
	return m_temp_args;
}

void TypeRef::SetID(std::string&& base_id)
{
	m_base_id = std::move(base_id);
}

TypeRef& TypeRef::AddTempArgs(TypeRef&& arg)
{
	m_temp_args.push_back(std::move(arg));
	return *this;
}

bool TypeRef::operator==(const TypeRef& ref) const
{
	return m_base_id == ref.m_base_id && m_temp_id == ref.m_temp_id && m_temp_args == ref.m_temp_args;
}

bool TypeRef::operator!=(const TypeRef& ref) const
{
	return !operator==(ref);
}

bool TypeRef::IsTemplate() const
{
	return !m_temp_id.empty();
}

const std::string& TypeRef::TemplateID() const
{
	return m_temp_id;
}

TypeRef& TypeRef::SetTemplateID(const std::string&& id)
{
	m_temp_id = std::move(id);
	return *this;
}

void TypeRef::FromTemplateRef(const libclx::Cursor& ref, const libclx::Type& type)
{
	assert(ref.Kind() == CXCursor_TemplateRef);
	
	m_temp_id = ref.Referenced().USR();
	FromType(type);
}

void TypeRef::FromType(const libclx::Type& type)
{
	m_name      = type.Declaration().DisplayName();
	m_base_id   = type.Declaration().USR();
	
	for (auto&& arg : type.TemplateArguments())
		m_temp_args.emplace_back(arg);
}

TypeRef::TypeRef(const std::string& base) :
	m_base_id{base}
{
}

const std::string& TypeRef::Name() const
{
	return m_name;
}

void TypeRef::SetName(std::string&& name)
{
	m_name = std::move(name);
}

std::ostream& operator<<(std::ostream& os, const TypeRef& ref)
{
	return os << ref.ID();
}

} // end of namespace
