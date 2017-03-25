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

ClassRef::ClassRef(const libclx::Cursor& cursor)
{
	switch (cursor.Kind())
	{
	case CXCursor_CXXBaseSpecifier: FromBaseSpecifier(cursor);
		break;
	
	case CXCursor_FieldDecl:    FromFieldDecl(cursor);
		break;
		
	default:
		break;
	}
}

void ClassRef::FromBaseSpecifier(const libclx::Cursor& cursor)
{
	assert(cursor.Kind() == CXCursor_CXXBaseSpecifier);
	m_name = cursor.DisplayName();
	
	// Iterating the TypeRef under the specifier.
	// It works for both template or non-template base classes.
	cursor.Visit([this](libclx::Cursor dec, libclx::Cursor parent)
	{
		switch (dec.Kind())
		{
		case CXCursor_TemplateRef:
			// TODO: handle multiple level of templates
			if (m_temp_id.empty())
			{
				assert(m_base_id.empty());
				m_base_id = parent.Referenced().USR();
				m_temp_id = dec.Referenced().USR();
			}
			break;
		
		case CXCursor_TypeRef:
			if (m_temp_id.empty())
				m_base_id = dec.Referenced().USR();
			else
				m_temp_args.push_back(dec.Referenced().USR());
			break;
		
		default:
			break;
		}
	});
}

void ClassRef::FromFieldDecl(const libclx::Cursor& cursor)
{
	assert(cursor.Kind() == CXCursor_FieldDecl);
	
	m_name = cursor.Type().Declaration().DisplayName();
	cursor.Visit([this](libclx::Cursor child, libclx::Cursor cursor)
	{
		auto type = cursor.Type();
		switch (child.Kind())
		{
		case CXCursor_TemplateRef:
			m_temp_id = child.Referenced().USR();
			m_base_id = type.Declaration().USR();
			for (auto&& arg : cursor.Type().TemplateArguments())
			{
				std::cout << "arg = " << arg.Declaration().USR() << std::endl;
				m_temp_args.push_back(arg.Declaration().USR());
			}
			break;
		
		case CXCursor_TypeRef:
			m_base_id = child.Referenced().USR();
			break;
		
		default:
			break;
		}
	});
	std::cout << m_base_id << std::endl;
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
