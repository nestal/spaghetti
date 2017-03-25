/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/18/17.
//

#pragma once

#include <vector>
#include <string>
#include <iosfwd>

namespace libclx {
class Cursor;
}

namespace codebase {

class EntityMap;

class ClassRef
{
public:
	ClassRef() = default;
	
	ClassRef(const std::string& base);
	explicit ClassRef(const libclx::Cursor& cursor);
	
	const std::string& ID() const;
	void SetID(std::string&& base_id);
	
	const std::string& Name() const;
	void SetName(std::string&& name);
	
	const std::string& TemplateID() const;
	ClassRef& SetTemplateID(const std::string&& id);
	
	const std::vector<std::string>& TempArgs() const;
	ClassRef& AddTempArgs(std::string&& arg);
	
	bool IsTemplate() const;
	
	bool operator==(const ClassRef& ref) const;
	bool operator!=(const ClassRef& ref) const;
	
private:
	void Visit(const libclx::Cursor& cursor);
	
private:
	//! USR of the class or template instantiation
	std::string m_base_id;
	
	//! USR of the class template that instantiate the class, or empty if non-template
	std::string m_temp_id;
	
	//! Name of the class, without namespace (i.e. Cursor::Spelling())
	std::string m_name;
	
	//! USR of template arguments, empty if non-template
	std::vector<std::string> m_temp_args;
};

std::ostream& operator<<(std::ostream& os, const ClassRef& ref);
	
} // end of namespace
