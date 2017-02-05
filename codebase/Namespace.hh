/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#pragma once

#include "DataType.hh"
#include "EntityVec.hh"
#include "Variable.hh"

namespace libclx {
class Cursor;
}

namespace codebase {

class Namespace : public Entity
{
public:
	Namespace();
	Namespace(libclx::Cursor cursor, const std::string& parent);
	
	const std::string& Name() const override;
	const std::string& ID() const override;
	std::string Type() const override;
	const std::string& Parent() const override;
	
	void Visit(libclx::Cursor cursor);
	
	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	Entity* Child(std::size_t idx) override;
	std::size_t IndexOf(const Entity* child) const override;
	
private:
	std::string m_name;
	std::string m_usr;
	std::string m_parent;
	
	EntityVec<Namespace> m_ns;
	EntityVec<DataType>  m_types;
	EntityVec<Variable>  m_vars;
};
	
} // end of namespace
