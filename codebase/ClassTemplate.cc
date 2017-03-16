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

void ClassTemplate::Visit(libclx::Cursor self)
{
	DataType::Visit(self);
	
	std::cout << self.Spelling() << " = " << self.GetDefinition().Type().Kind() << std::endl;
	
	for (auto&& arg : self.GetDefinition().Type().TemplateArguments())
	{
		std::cout << arg << " " << arg.Declaration().USR() << std::endl;
	}
}

/**
 * \brief Instantiate a template by replacing all its template parameters with the arguments
 * \param args  template arguments, i.e. the actual types that will replace the template parameters
 * \return the instantiated class
 */
std::unique_ptr<DataType> ClassTemplate::Instantiate(const std::vector<DataType>&)
{
	return std::unique_ptr<DataType>();
}
	
} // end of namespace
