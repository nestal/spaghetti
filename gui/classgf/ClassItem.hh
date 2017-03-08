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
#include <QGraphicsObject>

#include "BaseItem.hh"

#include <memory>

class QGraphicsSimpleTextItem;
class QSizeF;
class QStaticText;

class SizeGripItem;

namespace codebase {
class DataType;
class Entity;
class EntityMap;
}

namespace gui {
namespace classgf {

class Edge;
struct ItemRenderingOptions;

class ClassItem : public QGraphicsObject, public BaseItem
{
	Q_OBJECT
	
public:
	ClassItem(const codebase::DataType& class_, QObject *model, const QPointF& pos, const QSizeF& size = {225.0, 175.0});
	~ClassItem();
	
	QGraphicsItem* GraphicsItem() override;
	const QGraphicsItem* GraphicsItem() const override;
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	const std::string& ID() const;
	const codebase::DataType& DataType() const;

	static constexpr int Type = static_cast<int>(ItemType::class_item);
	int type() const override;
	
	QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
	
	ItemRelation RelationOf(const BaseItem *other) const override;
	classgf::ItemType ItemType() const override;
	void Update(const codebase::EntityMap *map);
	
	bool IsChanged() const override;
	void MarkUnchanged();

	void Resize(const QRectF& rect);
	void ShowTooltip(const QPointF& pos);
	
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	
signals:
	void OnJustChanged(ClassItem *self);
	
private:
	void ComputeSize(qreal content_height, qreal name_height, qreal field_height);
	void OnPositionChanged();
	void OnSelectedChange(bool selected);
	void Normalize();
	void DrawBox(QPainter *painter, const ItemRenderingOptions& setting);
	QStaticText NameText(const QSizeF& content, QFont& font);
	qreal Margin(const QFontMetrics& name_font, qreal factor) const;
	
	template <typename Member>
	auto DrawMember(QPainter *painter, const Member& member, const QPointF& pos, qreal width, const QFontMetrics& met);
	
	template <typename DrawFunc>
	void DrawUnScaled(QPainter *painter, const QPointF& pos, DrawFunc func);
	
	QString NameWithNamespace() const;
	
private:
	class Resizer;
	
	const codebase::DataType *m_class{};
	std::string        m_class_id;
	QRectF             m_bounding;

	std::size_t        m_show_function{0}, m_show_field{0};
	mutable bool       m_changed{false};
	
	std::unique_ptr<SizeGripItem> m_grip;
	
	enum class MouseActionWhenRelease {select, deselect, grip, none} ;
	MouseActionWhenRelease m_release_action{MouseActionWhenRelease::none};
		
	static const qreal m_margin;
};
	
}} // end of namespace
