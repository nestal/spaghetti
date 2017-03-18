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

void ClassTemplate::Visit(const libclx::Cursor& self)
{
	DataType::Visit(self);
	
	self.Visit([this, self](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_TemplateTypeParameter:
		{
//			std::cout << Name() << " has template parameter: " << child.Spelling() << " " << child.USR() << " " << child.Type() << std::endl;
			m_args.insert({child.Spelling(), child.USR()});
			break;
		}
		
		default:
			break;
		}
	});
}

/**
 * \brief Instantiate a template by replacing all its template parameters with the arguments
 * \param args  template arguments, i.e. the actual types that will replace the template parameters
 * \return the instantiated class
 */
std::unique_ptr<DataType> ClassTemplate::Instantiate(const std::vector<std::string>& args) const
{
	return std::unique_ptr<DataType>();
}
	
} // end of namespace
