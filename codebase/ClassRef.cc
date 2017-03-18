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

#include <ostream>
#include <cassert>

namespace codebase {


const std::string& ClassRef::ID() const
{
	return m_base_id;
}

const std::vector<std::string>& ClassRef::TempArgs() const
{
	return m_temp_args;
}

void ClassRef::SetID(std::string&& base_id)
{
	m_base_id = std::move(base_id);
}

ClassRef& ClassRef::AddTempArgs(std::string&& arg)
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
	if (cursor.Kind() == CXCursor_CXXBaseSpecifier)
		FromBaseCursor(cursor);
}

void ClassRef::FromBaseCursor(const libclx::Cursor& cursor)
{
	m_name = cursor.Spelling();
	
	// Iterating the TypeRef under the specifier.
	// It works for both template or non-template base classes.
	cursor.Visit([this](libclx::Cursor dec, libclx::Cursor parent)
	{
		switch (dec.Kind())
		{
		case CXCursor_TemplateRef:
			m_base_id = parent.Referenced().USR();
			m_temp_id = dec.Referenced().USR();
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
