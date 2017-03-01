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
		
//		item->ReCreateChildren(rect.width(), rect.height(), true);
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
/*
void ClassItem::ReCreateChildren(qreal width, qreal height, bool force_size)
{
	// remove all children
	delete m_name; m_name = nullptr;
	m_fields.clear();
	
	m_name = new QGraphicsSimpleTextItem{QString::fromStdString(m_class->Name()), this};
	
	// use a bold font for class names
	auto font = m_name->font();
	font.setBold(true);
	m_name->setFont(font);
		
	auto bounding = m_name->boundingRect().size();
	
	// assume all text items are of the same height
	auto total_rows = static_cast<std::size_t>(height/bounding.height());
	
	// one row for the name
	assert(total_rows > 0);
	total_rows--;
	
	auto function_count = std::min(m_class->Functions().size(), total_rows);
	auto field_count    = std::min(m_class->Fields().size(), total_rows);
	
	if ( field_count <= total_rows/2 && function_count > total_rows/2)
		function_count = std::min(total_rows - field_count, function_count);
	else if (function_count <= total_rows/2 && field_count > total_rows/2)
		field_count = std::min(total_rows - function_count, field_count);
	else if (field_count > total_rows/2 && function_count > total_rows/2)
		field_count = function_count = total_rows/2;
	
	assert(function_count <= total_rows);
	assert(field_count    <= total_rows);
	assert(function_count + field_count <= total_rows);
	
	std::size_t index=0;
	for (auto&& func : m_class->Functions())
	{
		if (++index > function_count) break;
		CreateTextItem(&func, bounding, width);
	}
	index=0;
	for (auto&& field : m_class->Fields())
	{
		if (++index > field_count) break;
		CreateTextItem(&field, bounding, width);
	}
	m_show_function = function_count;
	
	if (force_size)
	{
		bounding.setHeight(std::max(bounding.height(), height));
		bounding.setWidth(std::max(bounding.width(), width));
	}
	
	// make all children center at origin
	for (auto child : childItems())
		child->moveBy(-bounding.width()/2, -bounding.height()/2);
	
	// initialize geometry
	prepareGeometryChange();
	m_bounding.setCoords(
		-bounding.width()/2-m_margin,
		-bounding.height()/2-m_margin,
		bounding.width()/2+m_margin,
		bounding.height()/2+m_margin
	);
}
*/
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
		QPointF{m_bounding.right(), name_rect.bottom()},
		QPointF{m_bounding.left(),  name_rect.bottom()}
	);
	
	QRectF text_rect{
		QPointF{content.left(),  name_rect.bottom()},
		QPointF{content.right(), name_rect.bottom() + field_font_met.height()}
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
		text_rect.adjust(0, field_font_met.height(), 0, field_font_met.height());
	}

	// line between class name and function
	painter->drawLine(
		QPointF{m_bounding.right(), text_rect.top()},
		QPointF{m_bounding.left(),  text_rect.top()}
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
		text_rect.adjust(0, field_font_met.height(), 0, field_font_met.height());
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
		new SizeGripItem{new Resizer, this};

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
/*
void ClassItem::CreateTextItem(const codebase::Entity *entity, QSizeF& bounding, qreal width)
{
	assert(entity);
	
	auto field_item = new QGraphicsSimpleTextItem{
		QFontMetrics{QFont{}}.elidedText(
			QString::fromStdString(entity->UML()),
			Qt::ElideRight,
			static_cast<int>(std::max(m_name->boundingRect().width(), width)),
			0
		),
		this
	};
	field_item->moveBy(0, bounding.height());
	
	auto rect = field_item->boundingRect();
	bounding.rheight() += rect.height();
	bounding.rwidth()   = std::max(bounding.width(), rect.width());
	
	m_fields.emplace_back(field_item);
}
*/
void ClassItem::Update(const codebase::EntityMap *map)
{
	assert(map);
	m_class = map->TypedFind<codebase::DataType>(m_class_id);
	
	// remove all edges, the model will re-add them later
	ClearEdges();
	
//	ReCreateChildren(m_bounding.width(), m_bounding.height(), true);
}

void ClassItem::ComputeSize(const QRectF& content, const QFontMetrics& name_font, const QFontMetrics& field_font)
{
	auto total_rows = static_cast<std::size_t>((content.height() - name_font.height())/field_font.height());
	std::cout << "total rows = " << total_rows << std::endl;
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
	
}} // end of namespace
