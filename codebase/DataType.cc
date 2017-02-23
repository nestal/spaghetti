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

#include <iostream>

namespace codebase {

DataType::DataType(libclx::Cursor cursor, const std::string& parent) :
	EntityVec{cursor.Spelling(), cursor.USR(), parent}
{
	assert(cursor.Kind() == CXCursor_StructDecl || cursor.Kind() == CXCursor_ClassDecl);
	if (cursor.IsDefinition())
		m_definition = cursor.Location();
}


void DataType::Visit(libclx::Cursor self)
{
	assert(self.Kind() == CXCursor_StructDecl || self.Kind() == CXCursor_ClassDecl);
	assert(Name() == self.Spelling());
	assert(!ID().empty() && ID() == self.USR());
	
	if (self.IsDefinition() || m_definition == libclx::SourceLocation{})
		m_definition = self.Location();
	
	self.Visit([this](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_FieldDecl:
			Add(std::make_unique<Variable>(child, ID()));
			break;
			
		case CXCursor_CXXBaseSpecifier:
			m_base_classes.push_back(child.GetDefinition().USR());
			break;
			
		default:
//			if (!child.Location().IsFromSystemHeader())
//				std::cout << m_name << " " <<  child.Spelling() << ' ' << child.Kind() << std::endl;
			break;
		}
	});
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

boost::iterator_range<DataType::field_iterator> DataType::Fields() const
{
	return {m_fields.begin(), m_fields.end()};
}

std::ostream& operator<<(std::ostream& os, const DataType& c)
{
	os << "class: " << c.Name() << " (" << c.ID() << ")\n";
	for (auto&& field : c.Fields())
		os << "\t" << field << '\n';
	return os;
}

} // end of namespace
