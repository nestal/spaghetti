/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#include "ClassRef.hh"
#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"
#include "libclx/Type.hh"

#include <ostream>
#include <cassert>

namespace codebase {

ClassRef::ClassRef(const libclx::Type& type)
{
	FromType(type);
}

ClassRef::ClassRef(const libclx::Cursor& cursor) :
	m_base_id{cursor.USR()},
	m_name{cursor.DisplayName()},
	m_type_kind{cursor.Type().Kind()}
{
	std::cout << "created ClassRef for " << cursor.KindSpelling() << " base(" << m_base_id << ") temp(" << m_temp_id << ") " << m_name << std::endl;
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

//	std::cout << "created ClassRef for " << cursor.KindSpelling() << " base(" << m_base_id << ") temp(" << m_temp_id << ") " << m_name << std::endl;
}

const std::string& ClassRef::ID() const
{
	return m_base_id;
}

const std::vector<ClassRef>& ClassRef::TempArgs() const
{
	return m_temp_args;
}

void ClassRef::SetID(std::string&& base_id)
{
	m_base_id = std::move(base_id);
}

ClassRef& ClassRef::AddTempArgs(ClassRef&& arg)
{
	m_temp_args.push_back(std::move(arg));
	return *this;
}

bool ClassRef::operator==(const ClassRef& ref) const
{
	return m_base_id == ref.m_base_id && m_temp_id == ref.m_temp_id && m_temp_args == ref.m_temp_args;
}

bool ClassRef::operator!=(const ClassRef& ref) const
{
	return !operator==(ref);
}

bool ClassRef::IsTemplate() const
{
	return !m_temp_id.empty();
}

const std::string& ClassRef::TemplateID() const
{
	return m_temp_id;
}

ClassRef& ClassRef::SetTemplateID(const std::string&& id)
{
	m_temp_id = std::move(id);
	return *this;
}

void ClassRef::FromTemplateRef(const libclx::Cursor& ref, const libclx::Type& type)
{
	assert(ref.Kind() == CXCursor_TemplateRef);
	
	m_temp_id = ref.Referenced().USR();
	FromType(type);
}

void ClassRef::FromType(const libclx::Type& type)
{
	m_name      = type.Declaration().DisplayName();
	m_base_id   = type.Declaration().USR();
	
	for (auto&& arg : type.TemplateArguments())
		m_temp_args.emplace_back(arg);
}

ClassRef::ClassRef(const std::string& base) :
	m_base_id{base}
{
}

const std::string& ClassRef::Name() const
{
	return m_name;
}

void ClassRef::SetName(std::string&& name)
{
	m_name = std::move(name);
}

std::ostream& operator<<(std::ostream& os, const ClassRef& ref)
{
	return os << ref.ID();
}

} // end of namespace
