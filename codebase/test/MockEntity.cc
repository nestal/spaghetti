/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "MockEntity.hh"
#include "codebase/EntityType.hh"

namespace codebase {
namespace ut {

MockEntity::MockEntity(size_t idx, const EntityVec* parent) :
	LeafEntity{"mock" + std::to_string(idx), "mock" + std::to_string(idx) + "ID", parent},
	m_index{idx}
{
}

std::size_t MockEntity::Index() const
{
	return m_index;
}

EntityType MockEntity::Type() const
{
	return EntityType::none;
}

bool MockEntity::IsUsed() const
{
	return true;
}

void MockEntity::CrossReference(EntityMap *map)
{
	LeafEntity::CrossReference(map);
}
	
}} // end of namespace
