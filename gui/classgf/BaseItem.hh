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

#include <vector>
#include <memory>

class QGraphicsItem;

namespace codebase {
class EntityMap;
}

namespace gui {
namespace classgf {

class Edge;
class Viewport;

class BaseItem
{
public:
	using EdgePtr = std::shared_ptr<Edge>;
	using edge_iterator = std::vector<EdgePtr>::const_iterator;

public:
	virtual ~BaseItem();
	
	virtual QGraphicsItem *GraphicsItem() = 0;
	virtual const QGraphicsItem *GraphicsItem() const = 0;
	
	virtual classgf::ItemType ItemType() const = 0;
	virtual ItemRelation RelationOf(const BaseItem *other) const = 0;
	virtual bool IsChanged() const = 0;
	virtual void Update(const codebase::EntityMap *code_base) = 0;
	
	// helper
	static const Viewport& CurrentViewport(QWidget *viewport) ;
	
	// edge operations
	void AddEdge(const EdgePtr& edge);
	bool HasEdgeWith(const BaseItem *item) const;
	boost::iterator_range<edge_iterator> Edges() const;
	void ClearEdges();
	
private:
	void RemoveEdgeWith(const BaseItem *other);

private:
	std::vector<EdgePtr> m_edges;
};
	
}} // end of namespace
