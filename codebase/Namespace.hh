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

class Namespace : public EntityVec
{
public:
	Namespace();
	Namespace(libclx::Cursor cursor, const Entity* parent);
	Namespace(Namespace&&) = default;
	Namespace(const Namespace&) = delete;
	
	Namespace& operator=(Namespace&) = default;
	Namespace& operator=(const Namespace&) = delete;
	
	std::string Type() const override;
	
	void Visit(libclx::Cursor cursor);
	
	void RemoveUnused() override;
	
private:
	std::vector<DataType*>  m_types;
	std::vector<Namespace*> m_ns;
	std::vector<Variable*>  m_vars;
};
	
} // end of namespace
