/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "DataType.hh"

#include "EntityType.hh"
#include "EntityMap.hh"
#include "Function.hh"
#include "Variable.hh"
#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"

#include <ostream>
#include <iostream>

namespace codebase {

DataType::DataType(const libclx::Cursor& cursor, const EntityVec *parent) :
	ParentScope{cursor, parent}
{
	assert(cursor.Kind() == CXCursor_StructDecl
		|| cursor.Kind() == CXCursor_ClassDecl
		|| cursor.Kind() == CXCursor_ClassTemplate);
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}

DataType::DataType(
	const std::string& name,
	const std::string& usr,
	const libclx::SourceLocation def,
	const EntityVec *parent
) :
	ParentScope{name, usr, parent},
	m_definition{def}
{
}

void DataType::OnVisit(const libclx::Cursor& self)
{
	assert(self.Kind() == CXCursor_StructDecl
		|| self.Kind() == CXCursor_ClassDecl
		|| self.Kind() == CXCursor_ClassTemplate);
	assert(!ID().empty() && ID() == self.USR());
	
	if (self.IsDefinition() || m_definition == libclx::SourceLocation{})
		m_definition = self.Location();
	
	if (self.IsDefinition() && self.Location().IsFromMainFile())
		SetUsed();
}

void DataType::VisitChild(const libclx::Cursor& child, const libclx::Cursor& self)
{
	switch (child.Kind())
	{
	case CXCursor_CXXBaseSpecifier:
	{
		codebase::TypeRef base{child};
		
		// normally we don't have hundreds of base classes so sequential searches should be faster.
		// the order of the base classes is important, so we don't want to switch to set.
		if (std::find(m_bases.begin(), m_bases.end(), base) == m_bases.end())
			m_bases.push_back(base);
		
		break;
	}
	
	default: ParentScope::VisitChild(child, self);
		break;
	}
}

EntityType DataType::Type() const
{
	return EntityType::data_type;
}

libclx::SourceLocation DataType::Location() const
{
	return m_definition;
}

/**
 * \brief Get ID (USR) of all base classes
 * \return A range of iterator to base classes ID as string.
 */
boost::iterator_range<DataType::idvec_iterator> DataType::BaseClasses() const
{
	return {m_bases.begin(), m_bases.end()};
}

bool DataType::IsBaseOf(const DataType& other) const
{
	return std::find_if(
		other.m_bases.begin(),
		other.m_bases.end(),
		[this](auto&& ref) { return ref.ID() == this->ID(); }
	) != other.m_bases.end();
}

bool DataType::IsUsedInMember(const DataType& other) const
{
	auto fields = other.Fields();
	return std::find_if(fields.begin(), fields.end(),[this](auto&& field)
	{
		return field.ClassType() == this;
	}) != fields.end();
}

std::ostream& operator<<(std::ostream& os, const DataType& c)
{
	os << "class: " << c.Name() << " (" << c.ID() << ")\n";
	for (auto&& field : c.Fields())
		os << "\t" << field << '\n';
	return os;
}

void DataType::CrossReference(EntityMap *map)
{
	assert(map);

	// instantiate base class, if any
	for (auto&& base : m_bases)
	{
		if (base.IsTemplate() && !base.ID().empty() && base.ID() != base.TemplateID())
			map->Instantiate(base, IsUsed());
			
	}
	
	// instantiate data members
	for (auto&& field : m_fields)
	{
		auto type = field->TypeRef();
		if (type.IsTemplate() && !type.ID().empty() && type.ID() != type.TemplateID())
			map->Instantiate(type, IsUsed());
	}
	
	MarkBaseClassUsed(map);
}

void DataType::MarkBaseClassUsed(EntityMap *map)
{
	// mark base class as used
	if (IsUsed())
	{
		for (auto& base : m_bases)
		{
			auto base_entity = map->TypedFind<DataType>(base.ID());
			
			// TODO: support typedef base classes
			if (base_entity && !base_entity->IsUsed())
			{
				base_entity->SetUsed();
				base_entity->MarkBaseClassUsed(map);
			}
		}
	}
}

void DataType::AddBase(const codebase::TypeRef& base)
{
	m_bases.push_back(base);
}

void DataType::AddField(const Variable& var)
{
	EntityVec::Add(m_fields, std::make_unique<Variable>(var, this));
}

codebase::TypeRef DataType::GetTypeRef() const
{
	return codebase::TypeRef{ID()}.SetName(Name());
}

} // end of namespace
