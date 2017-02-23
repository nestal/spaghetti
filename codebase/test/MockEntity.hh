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

#include "codebase/Entity.hh"

namespace codebase {
namespace ut {

class MockEntity : public LeafEntity
{
public:
	MockEntity(std::size_t idx = 0, const Entity* parent = {});
	
	const std::string& Name() const override;
	const Entity* Parent() const override;
	const std::string& ID() const override;
	std::string Type() const override;
	
	std::size_t Index() const;
	
	bool IsUsed() const override;
	void RemoveUnused() override;
	
private:
	std::size_t m_index{};
	std::string m_name;
	const Entity* m_parent;
	std::string m_id;
};
	
}} // end of namespace
