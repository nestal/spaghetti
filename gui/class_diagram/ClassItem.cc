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

#include <QtGui/QFont>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsScene>

namespace gui {
namespace class_diagram {

const qreal ClassItem::m_margin{10.0};

const qreal ClassItem::m_max_width{200.0}, ClassItem::m_max_height{150.0};

ClassItem::ClassItem(const codebase::DataType& class_, const QPointF& pos, QObject *model) :
	QObject{model},
	m_class{&class_},
	m_class_id{class_.ID()}
{
	CreateChildren();

	// setting it here before setting ItemSendGeometryChanges will not trigger "is_changed"
	setPos(pos);
	
	// flags
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

ClassItem::~ClassItem() = default;

void ClassItem::CreateChildren()
{
	assert(!m_name);
	m_name = new QGraphicsSimpleTextItem{QString::fromStdString(m_class->Name()), this};
	
	// use a bold font for class names
	auto font = m_name->font();
	font.setBold(true);
	m_name->setFont(font);
		
	auto bounding = m_name->boundingRect().size();
	
	// assume all text items are of the same height
	auto total_rows = static_cast<std::size_t>(m_max_height/bounding.height());
	
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
	for (auto& func : m_class->Functions())
	{
		if (++index > function_count) break;
		CreateTextItem(&func, bounding);
	}
	index=0;
	for (auto& field : m_class->Fields())
	{
		if (++index > field_count) break;
		CreateTextItem(&field, bounding);
	}
	m_show_function = function_count;
	
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

QRectF ClassItem::boundingRect() const
{
	return m_bounding;
}

void ClassItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	// TODO: make it configurable
	painter->setPen(QPen{QColor{Qt::GlobalColor::magenta}});
	painter->setBrush(QBrush{QColor{isSelected() ? Qt::GlobalColor::cyan : Qt::GlobalColor::yellow}});
	
	// bounding rectangle
	painter->drawRect(m_bounding);
	
	// line between class name and function
	auto ypos = m_name->y() + m_name->boundingRect().height();
	painter->drawLine(
		QPointF{m_bounding.left(), ypos},
		QPointF{m_bounding.right(), ypos}
	);
	
	// line between functions and fields
	ypos += m_show_function * m_name->boundingRect().height();
	painter->drawLine(
		QPointF{m_bounding.left(), ypos},
		QPointF{m_bounding.right(), ypos}
	);
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

void ClassItem::CreateTextItem(const codebase::Entity *entity, QSizeF& bounding)
{
	assert(entity);
	
	auto field_item = new QGraphicsSimpleTextItem{
		QFontMetrics{QFont{}}.elidedText(
			QString::fromStdString(entity->UML()),
			Qt::ElideRight,
			static_cast<int>(std::max(m_name->boundingRect().width(), m_max_width)),
			0
		),
		this
	};
	field_item->moveBy(0, bounding.height());
	
	auto rect = field_item->boundingRect();
	bounding.rheight() += rect.height();
	bounding.rwidth()   = std::max(bounding.width(), rect.width());
}

void ClassItem::Update(const codebase::EntityMap *map)
{
	assert(map);
	m_class = map->TypedFind<codebase::DataType>(m_class_id);
	
	// remove all edges, the model will re-add them later
	ClearEdges();
	
	// remove all children
	for (auto child : childItems())
		delete child;
	m_name = nullptr;
	
	CreateChildren();
}

	
}} // end of namespace
