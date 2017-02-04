/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

#pragma once

#include "Entity.hh"
#include "libclx/Index.hh"

namespace codebase {

class Variable : public Entity
{
public:
	Variable(libclx::Cursor field);
	
	const std::string& Name() const override;
	const Entity* Parent() const override;
	const std::string& USR() const;
	std::string Type() const override;
	void Reparent(const Entity *parent) override;
	
	std::size_t ChildCount() const override;
	const Entity* Child(std::size_t idx) const override;
	std::size_t IndexOf(const Entity* child) const override;
	
	friend std::ostream& operator<<(std::ostream& os, const Variable& c);

private:
	std::string m_name;
	std::string m_usr;
	const Entity *m_parent{};
	libclx::Type   m_type;
};
	
} // end of namespace
