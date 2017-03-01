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

#include "codebase/DataType.hh"
#include "codebase/Variable.hh"
#include "codebase/Function.hh"

#include "gui/common/SizeGripItem.h"

#include <QtGui/QFont>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsScene>
#include <iostream>

// using https://github.com/cesarbs/sizegripitem to implement resize

namespace gui {
namespace class_diagram {

const qreal ClassItem::m_margin{10.0};

class ClassItem::Resizer : public SizeGripItem::Resizer
{
public:
	void operator()(QGraphicsItem* i, const QRectF& rect)
	{
		auto item = dynamic_cast<ClassItem*>(i);
		assert(item);
		
		std::cout << "width = " << rect.width( ) << " " << " height = " << rect.height() << std::endl;
		
		item->Resize(rect);
	}
};

ClassItem::ClassItem(const codebase::DataType& class_, const QPointF& pos, QObject *model) :
	QObject{model},
	m_class{&class_},
	m_class_id{class_.ID()}
{
	const qreal default_width{200.0}, default_height{150.0};

	m_bounding.setCoords(
		-default_width/2, -default_height/2,
		default_width/2,  default_height/2
	);
	
	// setting it here before setting ItemSendGeometryChanges will not trigger "is_changed"
	setPos(pos);
	
	// flags
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

ClassItem::~ClassItem() = default;

QRectF ClassItem::boundingRect() const
{
	return m_bounding;
}

void ClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	auto content = m_bounding.adjusted(m_margin, m_margin, -m_margin, -m_margin);
	
	// use bold font for name
	auto name_font = painter->font();
	name_font.setBold(true);
	QFontMetrics name_font_met{name_font}, field_font_met{painter->font()};
	ComputeSize(content, name_font_met, field_font_met);
	
	// adjust vertical margin
	auto total_height = name_font_met.height() + (m_show_field+m_show_function) * field_font_met.height();
	auto vspace_between_fields = (content.height() - total_height) / (m_show_field+m_show_function); // include space between name

	// TODO: make it configurable
	painter->setPen(Qt::GlobalColor::magenta);
	painter->setBrush(isSelected() ? Qt::GlobalColor::cyan : Qt::GlobalColor::yellow);
	
	// bounding rectangle
	painter->drawRect(m_bounding);
	
	QRectF name_rect;
	painter->setPen(Qt::GlobalColor::black);
	painter->drawText(
		QRectF{content.topLeft(), QPointF{content.right(), content.top()+name_font_met.height()}},
		Qt::AlignHCenter,
		QString::fromStdString(m_class->Name()),
		&name_rect
	);
	
	// line between class name and function
	painter->drawLine(
		QPointF{m_bounding.right(), name_rect.bottom() + vspace_between_fields/2},
		QPointF{m_bounding.left(),  name_rect.bottom() + vspace_between_fields/2}
	);
	
	QRectF text_rect{
		QPointF{content.left(),  name_rect.bottom() + vspace_between_fields},
		QPointF{content.right(), name_rect.bottom() + vspace_between_fields + field_font_met.height()}
	};
	
	// functions
	std::size_t index=0;
	for (auto&& func : m_class->Functions())
	{
		if (++index > m_show_function) break;
		
		painter->drawText(
			text_rect,
			field_font_met.elidedText(
				QString::fromStdString(func.UML()),
				Qt::ElideRight,
				content.width()
			)
		);
		text_rect.adjust(0, field_font_met.height() + vspace_between_fields, 0, field_font_met.height() + vspace_between_fields);
	}

	// line between class name and function
	painter->drawLine(
		QPointF{m_bounding.right(), text_rect.top() - vspace_between_fields/2},
		QPointF{m_bounding.left(),  text_rect.top() - vspace_between_fields/2}
	);
	
	index=0;
	for (auto&& field : m_class->Fields())
	{
		if (++index > m_show_field) break;
		painter->drawText(
			text_rect,
			field_font_met.elidedText(
				QString::fromStdString(field.UML()),
				Qt::ElideRight,
				content.width()
			)
		);
		text_rect.adjust(0, field_font_met.height() + vspace_between_fields, 0, field_font_met.height() + vspace_between_fields);
	}
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
	{
		if (!m_changed)
			emit OnJustChanged(this);
		
		m_changed = true;
		
		for (auto&& edge : Edges())
			edge->UpdatePosition();
	}
	else if (change == QGraphicsItem::ItemSelectedChange)
	{
		if (!m_grip)
			m_grip = std::make_unique<SizeGripItem>(new Resizer, this);
	}

	return value;
}

ItemRelation ClassItem::RelationOf(const BaseItem *other) const
{
	assert(m_class);
	assert(other);
	
	switch (other->ItemType())
	{
	case ItemType::class_item:
	{
		auto class_ = qgraphicsitem_cast<const ClassItem*>(other);
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

class_diagram::ItemType ClassItem::ItemType() const
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

void ClassItem::ComputeSize(const QRectF& content, const QFontMetrics& name_font, const QFontMetrics& field_font)
{
	auto total_rows = static_cast<std::size_t>((content.height() - name_font.height())/field_font.height());
	if (total_rows > 0)
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
	else
	{
		m_show_field = m_show_function = 0;
	}
}

void ClassItem::Resize(const QRectF& rect)
{
	prepareGeometryChange();
	m_bounding = rect;
}
	
}} // end of namespace
