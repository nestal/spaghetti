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

class ClassRef;
class ClassTemplate;
class InstantiatedDataType;

class ClassTemplate : public DataType
{
public:
	using DataType::DataType;
	
	void VisitChild(const libclx::Cursor& child, const libclx::Cursor& self) override;
	
	std::unique_ptr<DataType> Instantiate(const ClassRef& ref, bool used) const;
	
	EntityType Type() const override;

private:
	std::size_t Match(const std::string& usr) const;
	class Instance;
	
private:
	struct Param
	{
		std::string     name;
		std::string     usr;
	};
	std::vector<Param> m_param;
};

} // end of namespace
