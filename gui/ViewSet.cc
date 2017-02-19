/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/19/17.
//

#include "ViewSet.hh"

#include <cassert>

namespace gui {

void ViewSet::iterator::increment()
{
	assert(m_parent);
	assert(m_idx >= 0);
	m_idx++;
}

bool ViewSet::iterator::equal(const ViewSet::iterator& other) const
{
	assert(m_parent);
	assert(m_idx >= 0);
	assert(other.m_parent);
	assert(other.m_idx >= 0);
	return m_idx == other.m_idx && m_parent == other.m_parent;
}

common::ViewBase& ViewSet::iterator::dereference() const
{
	assert(m_parent);
	assert(m_idx >= 0);
	return dynamic_cast<common::ViewBase&>(*m_parent->widget(m_idx));
}

ViewSet::iterator ViewSet::begin()
{
	return {this, 0};
}

ViewSet::iterator ViewSet::end()
{
	return {this, count()};
}

} // end of namespace
