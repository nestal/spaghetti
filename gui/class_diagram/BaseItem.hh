/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/17/17.
//

#pragma once

#include "ItemType.hh"

#include <boost/range/iterator_range.hpp>

#include <QGraphicsItem>
#include <vector>

namespace codebase {
class EntityMap;
}

namespace gui {
namespace class_diagram {

class Edge;

class BaseItem : public QGraphicsItem
{
public:
	using QGraphicsItem::QGraphicsItem;
	
	virtual class_diagram::ItemType ItemType() const = 0;
	virtual ItemRelation RelationOf(const BaseItem *other) const = 0;
	virtual bool IsChanged() const = 0;
	virtual void Update(const codebase::EntityMap *code_base) = 0;

	using edge_iterator = std::vector<Edge*>::const_iterator;
	
	// edge operations
	void AddEdge(Edge *edge);
	bool HasEdgeWith(const BaseItem *item) const;
	boost::iterator_range<edge_iterator> Edges() const;
	void ClearEdges();
	
	template <typename Func>
	void RemoveEdgeWith(const BaseItem *other, Func func)
	{
		auto mid = std::partition(m_edges.begin(), m_edges.end(), [other, this](auto edge)
		{
			return edge->Other(this) != other;
		});
		std::for_each(mid, m_edges.end(), func);
		m_edges.erase(mid, m_edges.end());
	}

private:
	std::vector<Edge*> m_edges;
};

}} // end of namespace
