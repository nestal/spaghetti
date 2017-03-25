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

#include "ParentScope.hh"
#include "EntityVec.hh"

namespace libclx {
class Cursor;
}

namespace codebase {

class Variable;
class Function;
class ClassTemplate;
class DataType;

class Namespace : public ParentScope
{
public:
	Namespace();
	Namespace(const libclx::Cursor& cursor, const EntityVec* parent);
	Namespace(Namespace&&) = default;
	Namespace(const Namespace&) = delete;
	
	Namespace& operator=(Namespace&&) = default;
	Namespace& operator=(const Namespace&) = delete;
	
	EntityType Type() const override;
	
	void CrossReference(EntityMap *map) override;
	void MarkUsed() override;
	
private:
	void VisitChild(const libclx::Cursor& child, const libclx::Cursor& parent) override;
	
private:
	std::vector<Namespace*> m_ns;
};
	
} // end of namespace
