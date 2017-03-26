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

TypeRef::TypeRef(CXTypeKind kind) : TypeRef{{}, kind}
{
}

TypeRef::TypeRef(const std::string& base, CXTypeKind kind) :
	m_kind{kind},
	m_base_id{base},
	m_name{libclx::Type::KindSpelling(m_kind)}
{
}

TypeRef::TypeRef(const libclx::Type& type, const std::string& temp_id) :
	m_temp_id{temp_id}
{
	FromType(type);
}

TypeRef::TypeRef(const libclx::Cursor& cursor) :
	TypeRef{cursor.Type()}
{
	assert(
		cursor.Kind() == CXCursor_CXXBaseSpecifier ||
		cursor.Kind() == CXCursor_FieldDecl ||
		cursor.Kind() == CXCursor_ParmDecl ||
		cursor.Kind() == CXCursor_TypeAliasDecl
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
//		std::cout << child.KindSpelling() << " " << child.DisplayName() << std::endl;
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

TypeRef& TypeRef::AddTempArgs(TypeRef&& arg)
{
	m_temp_args.push_back(std::move(arg));
	return *this;
}

bool TypeRef::operator==(const TypeRef& ref) const
{
	return m_kind == ref.m_kind &&
		(
			// for record types (i.e. class/struct/union), need to compare the IDs of the
			// class/struct/union as well
			(m_kind != CXType_Record) ||
			(m_base_id == ref.m_base_id && m_temp_id == ref.m_temp_id && m_temp_args == ref.m_temp_args)
		);
}

bool TypeRef::operator!=(const TypeRef& ref) const
{
	return !operator==(ref);
}

// for convenience
bool operator==(const TypeRef& type, const std::string& id)
{
	return type.ID() == id;
}

bool operator!=(const TypeRef& type, const std::string& id)
{
	return type.ID() != id;
}

bool operator==(const std::string& id, const TypeRef& type)
{
	return type.ID() == id;
}

bool operator!=(const std::string& id, const TypeRef& type)
{
	return type.ID() != id;
}

bool TypeRef::IsTemplate() const
{
	return !m_temp_id.empty();
}

const std::string& TypeRef::TemplateID() const
{
	return m_temp_id;
}

TypeRef& TypeRef::SetTemplateID(const std::string& id)
{
	m_temp_id = id;
	return *this;
}

TypeRef& TypeRef::SetTemplate(const std::string& temp_id, const std::vector<TypeRef>& args)
{
	m_temp_id = temp_id;
	m_temp_args = args;
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
	auto actual = type;
	if (type.Kind() == CXType_Pointer)
		actual = type.PointeeType();
	
	m_name      = actual.Declaration().DisplayName();
	m_base_id   = actual.Declaration().USR();
	m_kind      = type.Kind();
	
	for (auto&& arg : actual.TemplateArguments())
		m_temp_args.push_back(TypeRef{arg});
}

const std::string& TypeRef::Name() const
{
	return m_name;
}

TypeRef& TypeRef::SetName(std::string&& name)
{
	m_name = std::move(name);
	return *this;
}

TypeRef& TypeRef::SetName(const std::string& name)
{
	m_name = name;
	return *this;
}

CXTypeKind TypeRef::Kind() const
{
	return m_kind;
}

std::ostream& operator<<(std::ostream& os, const TypeRef& ref)
{
	os << ref.ID() << '(' << libclx::Type::KindSpelling(ref.Kind()) << ':' << ref.TemplateID() << ':' << ref.TempArgs().size() << ':';
	
	for (auto&& arg : ref.TempArgs())
		os << arg << ",";
	
	return os << ')';
}

} // end of namespace
