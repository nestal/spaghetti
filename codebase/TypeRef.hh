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

#include "libclx/Type.hh"

#include <vector>
#include <string>
#include <iosfwd>

namespace libclx {
class Cursor;
}

namespace codebase {

class EntityMap;

class TypeRef
{
public:
	TypeRef(CXTypeKind kind = CXType_Invalid);
	TypeRef(const std::string& base, CXTypeKind kind = CXType_Record);
	explicit TypeRef(const libclx::Cursor& cursor);
	
	// TypeRef is a value type. Support copy and move.
	TypeRef(const TypeRef&) = default;
	TypeRef(TypeRef&&) = default;
	TypeRef& operator=(const TypeRef&) = default;
	TypeRef& operator=(TypeRef&&) = default;
	
	const std::string& ID() const;
	
	const std::string& Name() const;
	TypeRef& SetName(std::string&& name);
	TypeRef& SetName(const std::string& name);
	
	const std::string& TemplateID() const;
	TypeRef& SetTemplateID(const std::string& id);
	TypeRef& SetTemplate(const std::string& temp_id, const std::vector<TypeRef>& args);
	
	const std::vector<TypeRef>& TempArgs() const;
	TypeRef& AddTempArgs(TypeRef&& arg);
	
	bool IsTemplate() const;
	CXTypeKind Kind() const;
	
	bool operator==(const TypeRef& ref) const;
	bool operator!=(const TypeRef& ref) const;
	
private:
	void FromTemplateRef(const libclx::Cursor& cursor, const libclx::Type& type);
	void FromType(const libclx::Type& type);
	
	TypeRef(const libclx::Type& type);

private:
	//! For build-in types
	CXTypeKind m_kind{CXType_Invalid};
	
	//! USR of the class or template instantiation. Empty if build-in types.
	std::string m_base_id;
	
	//! Name of the class, without namespace (i.e. Cursor::Spelling())
	//! This is used to instantiate the class during CrossReference()
	std::string m_name{libclx::Type::KindSpelling(m_kind)};
	
	//! USR of the class template that instantiate the class, or empty if non-template
	std::string m_temp_id;
	
	//! USR of template arguments, empty if non-template
	std::vector<TypeRef> m_temp_args;
};

std::ostream& operator<<(std::ostream& os, const TypeRef& ref);

// for convenience
bool operator==(const TypeRef& type, const std::string& id);
bool operator!=(const TypeRef& type, const std::string& id);
bool operator==(const std::string& id, const TypeRef& type);
bool operator!=(const std::string& id, const TypeRef& type);

} // end of namespace
