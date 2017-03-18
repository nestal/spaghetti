/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/13/17.
//

#include "ClassTemplate.hh"

#include "libclx/Cursor.hh"
#include "libclx/Type.hh"

namespace codebase {

void ClassTemplate::VisitChild(const libclx::Cursor& child, const libclx::Cursor& self)
{
	switch (child.Kind())
	{
	case CXCursor_TemplateTypeParameter:
	{
		m_args.insert({child.Spelling(), child.USR()});
		break;
	}
	
	default:
		DataType::VisitChild(child, self);
		break;
	}
}

/**
 * \brief Instantiate a template by replacing all its template parameters with the arguments
 * \param args  template arguments, i.e. the actual types that will replace the template parameters
 * \return the instantiated class
 */
std::unique_ptr<InstantiatedDataType> ClassTemplate::Instantiate(const std::vector<std::string>& args) const
{
	auto inst = std::make_unique<InstantiatedDataType>(*this);
	
	for (auto&& base : BaseClasses())
	{
		auto at = m_args.find(base.ID());
		if (at != m_args.end())
			std::cout << at->second << std::endl;
	}
	return inst;
}

InstantiatedDataType::InstantiatedDataType(const ClassTemplate& temp) :
	DataType{temp.Name(), temp.ID(), temp.Location(), temp.Parent()},
	m_temp(&temp)
{
}

} // end of namespace
