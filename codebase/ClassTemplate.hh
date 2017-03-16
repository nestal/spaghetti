/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/13/17.
//

#pragma once

#include "DataType.hh"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <map>

namespace codebase {

class ClassTemplate : public DataType
{
public:
	using DataType::DataType;
	
	void Visit(libclx::Cursor self) override;
	
	std::unique_ptr<DataType> Instantiate(const std::vector<DataType>& args);
	
private:
	std::map<std::string, std::string> m_args;
};
	
} // end of namespace
