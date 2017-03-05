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
#include "Edge.hh"
#include "Viewport.hh"
#include "ItemRenderingOptions.hh"

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/Function.hh"

#include "gui/common/SizeGripItem.h"

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

const qreal ClassItem::m_margin{10.0};

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

qreal ClassItem::Margin(const QFontMetrics& name_font, qreal factor) const
{
	// assume content has 1 line
	auto remain_height = m_bounding.height() - name_font.height();
	return remain_height > name_font.height() ? m_margin/factor : std::max(remain_height/2, 0.0);
}

QStaticText ClassItem::NameText(const QTransform& transform, const QRectF& content, QFont& font)
{
	assert(content.width() > 0);
	assert(content.height() > 0);
	
	QStaticText text{QString::fromStdString(m_class->Name())};
	
	for (auto trial = 0 ; trial < 5 ; trial++)
	{
		QTextOption ops{Qt::AlignCenter};
		ops.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
		text.setTextOption(ops);
	
		text.setTextFormat(Qt::PlainText);
		text.setTextWidth(content.width());
		text.prepare(transform, font);

		// try to fit in box
		auto size = text.size();
		if (size.width() <= content.width() && size.height() <= content.height())
			break;
		
		// reduce font and retry
		auto font_factor = 0.0;
		
		// width OK, but height is not enough
		if (size.width() <= content.width() && size.height() > content.height())
			font_factor = content.height() / size.height();
		
		// height OK, but width not enough
		else if (size.height() <= content.height() && size.width() > content.width())
			font_factor = content.width() / size.width();
		
		else
		{
			assert(!std::isnan(size.width()));
			assert(!std::isnan(size.height()));
			
			assert(size.height() >= content.height());
			assert(size.width() >= content.width());
			font_factor = std::min(
				std::max(0.0, content.height() / size.height()),
				std::max(0.0, content.width() / size.width())
			);
		}
				
		assert(font_factor <= 1.0);
		auto new_size = font.pointSizeF() * font_factor;
		
		if (font_factor < 1.0 && new_size > 0)
			font.setPointSizeF(font.pointSizeF() * font_factor);
		else
			break;
	}
	
	return text;
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

void ClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *viewport)
{
	// assume the parent widget of the viewport is our ClassView
	// query the properties to get rendering parameters
	auto& view = CurrentViewport(viewport);
	auto& setting = view.Setting();
	
	DrawBox(painter, setting);
	
	// normalize font size
	auto name_font = setting.class_name_font;
	auto mem_font  = setting.class_member_font;
	name_font.setPointSizeF(setting.class_name_font.pointSize() / view.ZoomFactor());
	mem_font.setPointSizeF(setting.class_member_font.pointSize() / view.ZoomFactor());
	
	// normalize margin
	auto margin  = Margin(QFontMetrics{name_font}, view.ZoomFactor());
	auto content = m_bounding.adjusted(margin, margin, -margin, -margin);

	// fix bug found by Isis
	if (content.isEmpty())
		return;

	auto name = NameText(painter->transform(), content, name_font);
	ComputeSize(content, name.size(), QFontMetrics{mem_font});
	
	// don't let the member gets bigger than the name
	mem_font.setPointSizeF(std::min(name_font.pointSizeF(), mem_font.pointSizeF()));
	
	// adjust vertical margin
	QFontMetrics member_font_met{mem_font};
	auto total_height = name.size().height() + (m_show_field + m_show_function) * member_font_met.height();
	auto vspace_between_fields =
		(content.height() - total_height) / (m_show_field + m_show_function); // include space between name
	
	// draw class name in the middle of the box if there's no other member
	auto name_yoffset = (m_show_field == 0 && m_show_function == 0) ?
		(content.height() - name.size().height()) / 2 : 0.0;
	painter->setPen(Qt::GlobalColor::black);
	painter->setFont(name_font);
	painter->drawStaticText(QPointF{content.left(), content.top() + name_yoffset}, name);
	
	// line between class name and function
	auto name_line = content.top() + name.size().height() + vspace_between_fields / 2;
	
	QPointF text_pt{content.left(), content.top() + name.size().height() + vspace_between_fields};
	
	painter->setFont(mem_font);
	
	// functions
	std::size_t index = 0;
	for (auto&& func : m_class->Functions())
	{
		if (++index > m_show_function) break;
		text_pt = DrawMember(painter, func, text_pt, content.right(), vspace_between_fields, member_font_met);
	}
	
	// line between class name and function
	auto func_line = text_pt.y() - vspace_between_fields / 2;
	
	index = 0;
	for (auto&& field : m_class->Fields())
	{
		if (++index > m_show_field) break;
		text_pt = DrawMember(painter, field, text_pt, content.right(), vspace_between_fields, member_font_met);
	}
	
	QPen line_pen{setting.line_color};
	line_pen.setCosmetic(true);
	painter->setPen(line_pen);
	if (m_show_field > 0 || m_show_function > 0)
		painter->drawLine(
			QPointF{m_bounding.right(), name_line},
			QPointF{m_bounding.left(),  name_line}
		);
	if (m_show_field > 0)
		painter->drawLine(
			QPointF{m_bounding.right(), func_line},
			QPointF{m_bounding.left(),  func_line}
		);
}

template <typename Member>
QPointF ClassItem::DrawMember(QPainter *painter, const Member& member, const QPointF& pos, qreal right, qreal vspace, const QFontMetrics& met)
{
	QRectF out;
	painter->drawText(
		QRectF{pos, QPointF{right, pos.y()+met.height()}},
		Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip,
		met.elidedText(
			QString::fromStdString(member.UML()),
			Qt::ElideRight,
			static_cast<int>(right - pos.x())
		),
		&out
	);
	return QPointF{pos.x(), out.bottom() + vspace};
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

/**
 * \brief Determine how many member functions and variables (fields) that can be fitted in the box
 * \param content       the size of the box to fit in
 * \param name_font     the metric of the font to render the class name
 * \param field_font    the metric of the font to render the functions and fields
 */
void ClassItem::ComputeSize(const QRectF& content, const QSizeF& name, const QFontMetrics& field_font)
{
	// total number of function and fields that can be rendered
	auto total_rows = static_cast<std::size_t>((content.height() - name.height())/field_font.height());
	
	// determine how to distribute the space between function and fields
	if (content.height() > name.height() && total_rows > 0)
	{
		m_show_function = std::min(m_class->Functions().size(), total_rows);
		m_show_field    = std::min(m_class->Fields().size(),    total_rows);
		
		if (m_show_field <= total_rows / 2 && m_show_function > total_rows / 2)
			m_show_function = std::min(total_rows - m_show_field, m_show_function);
		else if (m_show_function <= total_rows / 2 && m_show_field > total_rows / 2)
			m_show_field = std::min(total_rows - m_show_function, m_show_field);
		else if (m_show_field > total_rows / 2 && m_show_function > total_rows / 2)
		{
			m_show_field    = total_rows / 2;
			m_show_function = total_rows - m_show_field;
		}
		
		assert(m_show_function <= total_rows);
		assert(m_show_field <= total_rows);
		assert(m_show_function + m_show_field <= total_rows);
	}
		
	// no space to render any function or fields
	else
		m_show_field = m_show_function = 0;
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
	}
		
	// prevent default handling
	event->accept();
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
	
}} // end of namespace
