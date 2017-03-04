/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/17/17.
//

#include "BaseItem.hh"

#include <QWidget>
#include "Edge.hh"
#include "Viewport.hh"
#include "Setting.hh"

namespace gui {
namespace classgf {

void BaseItem::AddEdge(Edge *edge)
{
	m_edges.push_back(edge);
}

bool BaseItem::HasEdgeWith(const BaseItem *item) const
{
	for (auto&& edge : m_edges)
	{
		if (edge->Other(this) == item)
			return true;
	}
	return false;
}

boost::iterator_range<BaseItem::edge_iterator> BaseItem::Edges() const
{
	return {m_edges.begin(), m_edges.end()};
}

void BaseItem::ClearEdges()
{
	m_edges.clear();
}

const Viewport& BaseItem::CurrentViewport(QWidget *viewport)
{
	static class MockViewport : public Viewport
	{
	public:
		const classgf::Setting& Setting() const override {return m_setting;}
		qreal ZoomFactor() const override {return 1.0;}
	
	private:
		classgf::Setting m_setting;
	} mock;
	
	auto view = (viewport && viewport->parentWidget()) ? dynamic_cast<Viewport*>(viewport->parentWidget()) : nullptr;
	return view ? *view : mock;
}
	
}} // end of namespace
