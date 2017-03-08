/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/31/17.
//

#include "ClassItem.hh"
#include "ClassLayout.hh"
#include "Edge.hh"
#include "Viewport.hh"
#include "ItemRenderingOptions.hh"

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/Function.hh"

#include "gui/common/SizeGripItem.h"
#include "gui/common/CommonIO.hh"

#include <QtGui/QFont>
#include <QtGui/QPainter>
#include <QtGui/QStaticText>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsDropShadowEffect>

#include <QDebug>
#include <iostream>
#include <cmath>

// using https://github.com/cesarbs/sizegripitem to implement resize

namespace gui {
namespace classgf {

class ClassItem::Resizer : public SizeGripItem::Resizer
{
public:
	QRectF operator()(QGraphicsItem* i, const QRectF& rect)
	{
		auto item = dynamic_cast<ClassItem*>(i);
		assert(item);
	
		item->Resize(rect);
		return item->boundingRect();
	}
};

ClassItem::ClassItem(const codebase::DataType& class_, QObject *model, const QPointF& pos, const QSizeF& size) :
	m_class{&class_},
	m_class_id{class_.ID()}
{
	setParent(model);
	
	m_bounding.setCoords(
		-size.width()/2, -size.height()/2,
		+size.width()/2, +size.height()/2
	);
	
	// setting it here before setting ItemSendGeometryChanges will not trigger "is_changed"
	setPos(pos);
	
	// flags
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
}

ClassItem::~ClassItem() = default;

QRectF ClassItem::boundingRect() const
{
	return m_bounding;
}

void ClassItem::DrawBox(QPainter *painter, const ItemRenderingOptions& setting)
{
	QLinearGradient g{m_bounding.topLeft(), m_bounding.bottomRight()};
	g.setColorAt(0, setting.class_box_color);
	g.setColorAt(1, setting.class_box_color2);
	painter->setBrush(g);
	
	// draw lines and bounding rectangle
	QPen line_pen{setting.line_color};
	line_pen.setCosmetic(true);
	painter->setPen(line_pen);
	painter->drawRect(m_bounding);
}

template <typename Member>
void ClassItem::DrawMember(QPainter *painter, const Member& member, const ClassLayout& layout, std::size_t index)
{
	auto zoom_factor = painter->transform().m11();
	
	auto mem_rect = layout.MemberRect(index);
	auto met = layout.MemberMetrics();
	auto pos = mem_rect.topLeft();
	
	QRectF rect{0, 0,
		mem_rect.width() * zoom_factor,
		mem_rect.height() * zoom_factor
	};
	DrawUnScaled(
		painter, pos, [painter, &rect, &met, &member]
		{
			painter->setPen(Qt::SolidLine);
			painter->drawText(
				rect,
				Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip,
				met.elidedText(
					QString::fromStdString(member.UML()),
					Qt::ElideRight,
					static_cast<int>(rect.width())
				)
			);
		}
	);
}

void ClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *viewport)
{
	auto t = painter->transform();
	assert(t.isAffine());
	assert(t.m11() == t.m22());
	
	// assume the parent widget of the viewport is our ClassView
	// query the properties to get rendering parameters
	auto& view = CurrentViewport(viewport);
	auto& setting = view.Setting();
	
	DrawBox(painter, setting);
	
	ClassLayout layout{
		QString::fromStdString(m_class->Name()),
		NameWithNamespace(),
		m_bounding,
		t.m11(),
		setting,
		m_class->Functions().size(),
		m_class->Fields().size()
	};

	painter->setPen(Qt::GlobalColor::black);
	painter->setFont(layout.NameFont());
	
	// reset transform to make the text size unaffected by zoom factor
	DrawUnScaled(
		painter, layout.NameRect().topLeft(), [&layout, painter]
		{
			painter->drawStaticText(0, 0, layout.Name());
		}
	);
	
	painter->setFont(layout.MemberFont());

	// functions
	std::size_t index = 0;
	for (auto&& func : m_class->Functions())
	{
		if (index >= layout.FunctionCount()) break;
		DrawMember(painter, func, layout, index);
		index++;
	}
	
	index = 0;
	for (auto&& field : m_class->Fields())
	{
		if (index >= layout.FieldCount()) break;
		DrawMember(painter, field, layout, layout.FunctionCount() + index);
		index++;
	}
	
	QPen line_pen{setting.line_color};
	line_pen.setCosmetic(true);
	painter->setPen(line_pen);
	if (layout.FieldCount() > 0 || layout.FunctionCount() > 0)
		painter->drawLine(layout.Header().bottomLeft(), layout.Header().bottomRight());
	
	if (layout.FieldCount() > 0)
		painter->drawLine(layout.Separator());
}

template <typename DrawFunc>
void ClassItem::DrawUnScaled(QPainter *painter, const QPointF& pos, DrawFunc func)
{
	painter->save();
	auto t = painter->transform();
	
	auto dx = pos.x() * t.m11();
	auto dy = pos.y() * t.m22();
	
	// set the scale to 1 but keep the translation
	painter->setTransform({
		1, 0, 0,
		0, 1, 0,
		t.m31() + dx, t.m32() + dy, 1
	});
	
	func();
	painter->restore();
}

const std::string& ClassItem::ID() const
{
	return m_class->ID();
}

const codebase::DataType& ClassItem::DataType() const
{
	return *m_class;
}

int ClassItem::type() const
{
	return Type;
}

QVariant ClassItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (change == QGraphicsItem::ItemPositionChange)
		OnPositionChanged();
	
	else if (change == QGraphicsItem::ItemSelectedChange)
		OnSelectedChange(value.toBool());

	return value;
}

void ClassItem::OnPositionChanged()
{
	if (!m_changed)
		emit OnJustChanged(this);
	
	m_changed = true;
	
	for (auto&& edge : Edges())
		edge->UpdatePosition();
}

void ClassItem::OnSelectedChange(bool selected)
{
	if (selected)
	{
		auto effect = new QGraphicsDropShadowEffect{this};
		effect->setBlurRadius(10);
		setGraphicsEffect(effect);
	}
	else
	{
		m_grip.reset();
		Normalize();
		setGraphicsEffect(nullptr);
	}
}

void ClassItem::Normalize()
{
	auto size = m_bounding.size();
	auto pos  = mapToScene(m_bounding.center());
	setPos(pos);
	m_bounding.setCoords(
		-size.width()/2, -size.height()/2,
		+size.width()/2, +size.height()/2
	);
}

ItemRelation ClassItem::RelationOf(const BaseItem *other) const
{
	assert(m_class);
	assert(other);
	
	switch (other->ItemType())
	{
	case ItemType::class_item:
	{
		auto class_ = dynamic_cast<const ClassItem*>(other);
		assert(class_ && class_->m_class);
		
		if (class_->m_class->IsBaseOf(*m_class))
			return ItemRelation::derived_class_of;
		
		else if (m_class->IsBaseOf(*class_->m_class))
			return ItemRelation::base_class_of;
		
		else if (m_class->IsUsedInMember(*class_->m_class))
			return ItemRelation::used_by_as_member;
		
		else if (class_->m_class->IsUsedInMember(*m_class))
			return ItemRelation::use_as_member;
		
		// fall through
	}
	default:    return ItemRelation::no_relation;
	}
}

classgf::ItemType ClassItem::ItemType() const
{
	return ItemType::class_item;
}

bool ClassItem::IsChanged() const
{
	return m_changed;
}

void ClassItem::MarkUnchanged()
{
	m_changed = false;
}

void ClassItem::Update(const codebase::EntityMap *map)
{
	assert(map);
	m_class = map->TypedFind<codebase::DataType>(m_class_id);
	
	// remove all edges, the model will re-add them later
	ClearEdges();
}

void ClassItem::Resize(const QRectF& rect)
{
	prepareGeometryChange();
	m_bounding = rect;
	
	OnPositionChanged();
}

QGraphicsItem *ClassItem::GraphicsItem()
{
	return this;
}

const QGraphicsItem *ClassItem::GraphicsItem() const
{
	return this;
}

void ClassItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		// the user clicks on this item, and started dragging it
		// before releasing it.
		// if the user just clicks and doesn't drag, we will select
		// the item when the click is released.
		// but if the user starts dragging, we need to move the item.
		// so we select the item now, without waiting for click released.
		if (m_release_action == MouseActionWhenRelease::select)
		{
			for (auto&& item : scene()->selectedItems())
				item->setSelected(false);
			setSelected(true);
		}
		
		auto distance = event->pos() - event->lastPos();

		// move all selected items
		for (auto&& item : scene()->selectedItems())
			item->moveBy(distance.x(), distance.y());

		// the item is now selected anyway, so we don't need to
		// do anything when click released.
		m_release_action = MouseActionWhenRelease::none;
	}
}

void ClassItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		// set the action to be done when the mouse is released base on the state when
		// the mouse is pressed
		if (!isSelected())
			m_release_action = MouseActionWhenRelease::select;
		
		else
			m_release_action = m_grip ? MouseActionWhenRelease::deselect : MouseActionWhenRelease::grip;
		
		// prevent default handling
		event->accept();
	}
	else
		QGraphicsObject::mouseMoveEvent(event);
}

void ClassItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	// if action is "select", we want to deselect all other items because selection is exclusive
	// if action is "deselect", other items are not selected anyway
	// if action is "grip", we don't want to keep other items being selected
	if (m_release_action != MouseActionWhenRelease::none)
	{
		for (auto&& item : scene()->selectedItems())
		{
			if (item != this)
				item->setSelected(false);
		}
	}
	
	switch (m_release_action)
	{
	case MouseActionWhenRelease::select:
		setSelected(true);
		break;
	case MouseActionWhenRelease::deselect:
		setSelected(false);
		break;
	case MouseActionWhenRelease::grip:
		if (!m_grip)
			m_grip = std::make_unique<SizeGripItem>(new Resizer, this);
		break;
		
	case MouseActionWhenRelease::none:
		break;
	}
	
	m_release_action = MouseActionWhenRelease::none;
	
	// prevent default handling
	event->accept();
}

QString ClassItem::NameWithNamespace() const
{
	auto result = QString::fromStdString(m_class->Name());
	for (const codebase::Entity *i = m_class->Parent(); i->Parent() != nullptr; i = i->Parent())
		result.prepend(QString::fromStdString(i->Name()) + "::");
	return result;
}

void ClassItem::ShowTooltip(const QPointF& pos)
{
	std::cout << m_class->Name() << " help at " << pos.x() << " " << pos.y() << std::endl;
}
	
}} // end of namespace
