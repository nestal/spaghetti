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
	
	self.Visit([this, self](libclx::Cursor child, libclx::Cursor)
	{
		switch (child.Kind())
		{
		case CXCursor_TemplateTypeParameter:
		{
			std::cout << Name() << " has template parameter: " << child.Spelling() << " " << child.USR() << " " << child.Type() << std::endl;
			m_args.insert({child.Spelling(), child.USR()});
			break;
		}
		
		case CXCursor_CXXBaseSpecifier:
		{
			auto base_id = child.GetDefinition().USR();
			
			// if the base ID is empty, it may be a template parameter
			if (base_id.empty())
			{
				auto it = m_args.find(child.Spelling());
				if (it != m_args.end())
					base_id = it->second;
			}
				
			std::cout << Name() << " template inherit from " << child.Spelling() << " \"" << base_id << "\" " << std::endl;
			break;
		}
		
		case CXCursor_TemplateRef:
		{
			child.Visit([](libclx::Cursor tref, libclx::Cursor)
			{
				std::cout << "tref: " << tref.Spelling() << " " << tref.KindSpelling() << std::endl;
			});
			break;
		}
		
		default:
		std::cout << "temp: " << Name() << " " << child.KindSpelling() << " " << child.Spelling() << std::endl;
			break;
		}
	});
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
