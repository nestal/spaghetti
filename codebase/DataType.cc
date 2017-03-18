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

#include "Function.hh"
#include "Variable.hh"
#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"

#include <ostream>
#include <iostream>

namespace codebase {

DataType::DataType(const libclx::Cursor& cursor, const Entity* parent) :
	ParentScope{cursor, parent}
{
	assert(cursor.Kind() == CXCursor_StructDecl || cursor.Kind() == CXCursor_ClassDecl || cursor.Kind() == CXCursor_ClassTemplate);
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}


void DataType::OnVisit(const libclx::Cursor& self)
{
	assert(self.Kind() == CXCursor_StructDecl
		|| self.Kind() == CXCursor_ClassDecl
		|| self.Kind() == CXCursor_ClassTemplate);
	
	if (self.IsDefinition() || m_definition == libclx::SourceLocation{})
		m_definition = self.Location();
}

void DataType::VisitChild(const libclx::Cursor& child, const libclx::Cursor& self)
{
	ParentScope::VisitChild(child, self);
	switch (child.Kind())
	{
	case CXCursor_CXXBaseSpecifier:
	{
		ClassRef base{child};
		
		// normally we don't have hundreds of base classes so sequential searches should be faster
		// the order of the base classes is important, so we don't want to switch to set
		if (std::find(m_base_classes.begin(), m_base_classes.end(), base) == m_base_classes.end())
			m_base_classes.push_back(base);
		
		break;
	}
	
	default: break;
	}
}

void DataType::AfterVisitingChild(const libclx::Cursor& self)
{
	// mark self and all children as used, after creating the children
	if (IsUsed() || (self.IsDefinition() && self.Location().IsFromMainFile()))
		MarkUsed();
}

std::string DataType::Type() const
{
	return "Class";
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
	return {m_base_classes.begin(), m_base_classes.end()};
}

bool DataType::IsBaseOf(const DataType& other) const
{
	return std::find_if(
		other.m_base_classes.begin(),
		other.m_base_classes.end(),
		[this](auto&& ref){return ref.ID() == this->ID();}
	) != other.m_base_classes.end();
}

bool DataType::IsUsedInMember(const DataType& other) const
{
	auto fields = other.Fields();
	return std::find_if(fields.begin(), fields.end(), [myid = ID()](auto field)
	{
		return field.TypeID() == myid;
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
	MarkBaseClassUsed(map);
	
	MarkUsed();
}

void DataType::MarkBaseClassUsed(EntityMap *map)
{
	// mark base class as used
	if (IsUsed())
	{
		for (auto& base : m_base_classes)
		{
			auto base_entity = dynamic_cast<DataType*>(map->Find(base.ID()));
			
			// TODO: support typedef base classes
			if (base_entity && !base_entity->IsUsed())
			{
				base_entity->MarkUsed();
				base_entity->MarkBaseClassUsed(map);
			}
		}
	}
}

} // end of namespace
