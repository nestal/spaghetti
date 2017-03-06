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
#include "ItemRenderingOptions.hh"

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
	class MockViewport : public Viewport
	{
	public:
		const ItemRenderingOptions& Setting() const override {return m_setting;}
		qreal ZoomFactor() const override {return 1.0;}
		QTransform Transform() const override { return{}; }
	
	private:
		ItemRenderingOptions m_setting;
	};
	static const MockViewport mock;
	
	const auto *view = (viewport && viewport->parentWidget()) ? dynamic_cast<Viewport*>(viewport->parentWidget()) : nullptr;
	return view ? *view : mock;
}
	
}} // end of namespace
