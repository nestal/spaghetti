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

#include "libclx/Cursor.hh"
#include <ostream>
#include <iostream>

namespace codebase {

DataType::DataType(libclx::Cursor cursor, const Entity* parent) :
	EntityVec{cursor.Spelling(), cursor.USR(), parent}
{
	assert(cursor.Kind() == CXCursor_StructDecl || cursor.Kind() == CXCursor_ClassDecl || cursor.Kind() == CXCursor_ClassTemplate);
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}


void DataType::Visit(libclx::Cursor self)
{
	assert(self.Kind() == CXCursor_StructDecl || self.Kind() == CXCursor_ClassDecl || self.Kind() == CXCursor_ClassTemplate);
	assert(Name() == self.Spelling());
	assert(!ID().empty() && ID() == self.USR());
	
	if (self.IsDefinition() || m_definition == libclx::SourceLocation{})
		m_definition = self.Location();
	
//	std::cout << "class: " << Name() << " " << self.Kind() << " " << Location() << std::endl;
	
	self.Visit([this](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_FieldDecl:
			AddUnique(m_fields, child.USR(), child, this);
//			std::cout << Name() << " has field: \"" <<  child.Spelling() << "\" " << child.KindSpelling() << std::endl;
			break;
		
		// nested classes
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl:
		case CXCursor_ClassTemplate:
			AddUnique(m_nested_types, child.USR(), child, this)->Visit(child);
			break;
		
		case CXCursor_CXXBaseSpecifier:
		{
			// normally we don't have hundreds of base classes so sequential searches should be faster
			// the order of the base classes is important, so we don't want to switch to set
			if (Name() == "ViewSet")
			std::cout << Name() << " inherits from: \"" <<  child.Spelling() << "\" " << child.KindSpelling() << std::endl;
			auto&& base = child.GetDefinition().USR();
			if (std::find(m_base_classes.begin(), m_base_classes.end(), base) == m_base_classes.end())
				m_base_classes.push_back(base);
			break;
		}
		case CXCursor_CXXMethod:
//			std::cout << Name() << " has method: \"" <<  child.Spelling() << "\" \"" << child.KindSpelling() << "\" " << p.USR() << std::endl;
			AddUnique(m_functions, child.USR(), child, this);
			break;
			
		case CXCursor_TypeRef:
//			std::cout << Name() << " has type ref: \"" <<  child.Spelling() << "\" " << child.KindSpelling() << " " << child.Referenced().Spelling() << " " << child.Location() << std::endl;
			break;
		
		default:
//			if (!child.Location().IsFromSystemHeader())
//				std::cout << Name() << " has child: \"" <<  child.Spelling() << "\" " << child.KindSpelling() << " " << child.Location() << " " << child.Type() <<  std::endl;
			break;
		}
	});

	// mark self and all children as used, after creating the children
	if (IsUsed() || (self.IsDefinition() && self.Location().IsFromMainFile()))
		MarkUsed();
}

void DataType::MarkUsed()
{
	EntityVec::MarkUsed();
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
	return std::find(other.m_base_classes.begin(), other.m_base_classes.end(), ID()) !=
		other.m_base_classes.end();
}

bool DataType::IsUsedInMember(const DataType& other) const
{
	auto fields = other.Fields();
	auto& id = ID();
	return std::find_if(fields.begin(), fields.end(), [&id](auto field)
	{
		return field.TypeID() == id;
	}) != fields.end();
}

boost::iterator_range<DataType::field_iterator> DataType::Fields() const
{
	return {m_fields.begin(), m_fields.end()};
}

boost::iterator_range<DataType::function_iterator> DataType::Functions() const
{
	return {m_functions.begin(), m_functions.end()};
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
	
	EntityVec::MarkUsed();
}

void DataType::MarkBaseClassUsed(EntityMap *map)
{
	// mark base class as used
	if (IsUsed())
	{
		for (auto& base : m_base_classes)
		{
			auto base_entity = dynamic_cast<DataType*>(map->Find(base));
			
			// TODO: support typedef base classes
			if (base_entity)
			{
				base_entity->MarkUsed();
				base_entity->MarkBaseClassUsed(map);
			}
		}
	}
}

void DataType::VisitFunction(libclx::Cursor func)
{
	auto it = FindByID(m_functions, func.USR());
	
	// the definition of a member function should come after its declaration
	if (it != m_functions.end())
		(*it)->Visit(func);
}

const codebase::Function& DataType::Function(std::size_t idx) const
{
	return *m_functions.at(idx);
}

const codebase::Variable& DataType::Field(std::size_t idx) const
{
	return *m_fields.at(idx);
}
	
} // end of namespace
