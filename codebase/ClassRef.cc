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

namespace codebase {


const std::string& ClassRef::BaseID() const
{
	return m_base_id;
}

const std::vector<std::string>& ClassRef::TempArgs() const
{
	return m_temp_args;
}

void ClassRef::SetBaseID(std::string&& base_id)
{
	m_base_id = std::move(base_id);
}

void ClassRef::AddTempArgs(std::string&& arg)
{
	m_temp_args.push_back(std::move(arg));
}

bool ClassRef::operator==(const ClassRef& ref) const
{
	return m_base_id == ref.m_base_id && m_temp_args == ref.m_temp_args;
}

bool ClassRef::operator!=(const ClassRef& ref) const
{
	return !operator==(ref);
}

std::ostream& operator<<(std::ostream& os, const ClassRef& ref)
{
	os << ref.BaseID();
	
	auto&& args = ref.TempArgs();
	if (!args.empty())
	{
		os << '(';
		for (auto&& arg : ref.TempArgs())
			os << arg << ',';
		os << ')';
	}
	return os;
}

} // end of namespace
