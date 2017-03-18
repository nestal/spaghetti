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

void ClassRef::AddTempArgs(std::string&& arg)
{
	m_temp_args.push_back(std::move(arg));
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

void ClassRef::SetTemplateID(const std::string&& id)
{
	m_temp_id = std::move(id);
}

std::ostream& operator<<(std::ostream& os, const ClassRef& ref)
{
	return os << ref.ID();
}

} // end of namespace
