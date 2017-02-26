/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#pragma once

#include <QtCore/QObject>
#include "BaseItem.hh"

class QGraphicsSimpleTextItem;
class QSizeF;

namespace codebase {
class DataType;
class Entity;
class EntityMap;
}

namespace gui {
namespace class_diagram {

class Edge;

class ClassItem : public QObject, public BaseItem
{
	Q_OBJECT

public:
	ClassItem(const codebase::DataType& class_, const QPointF& pos, QObject *model);
	~ClassItem();
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	const std::string& ID() const;
	const codebase::DataType& DataType() const;

	static constexpr int Type = static_cast<int>(ItemType::class_item);
	int type() const override;
	
	QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
	
	void AddEdge(Edge *edge);
	
	ItemRelation RelationOf(const BaseItem *other) const override;
	class_diagram::ItemType ItemType() const override;
	void Update(const codebase::EntityMap *code_base);
	
	bool IsChanged() const override;
	void MarkUnchanged();

signals:
	void OnJustChanged(ClassItem *self);
	
private:
	void CreateTextItem(const codebase::Entity *entity, QSizeF& bounding);
	void CreateChildren();
	
private:
	const codebase::DataType *m_class;
	QGraphicsSimpleTextItem  *m_name;
	
	std::string        m_class_id;
	QRectF             m_bounding;
	std::vector<Edge*> m_edges;

	std::size_t        m_show_function{0};
	mutable bool       m_changed{false};
		
	static const qreal m_margin, m_max_width, m_max_height;
};
	
}} // end of namespace
