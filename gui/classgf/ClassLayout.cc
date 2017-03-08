/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/8/17.
//

#include "ClassLayout.hh"
#include "ItemRenderingOptions.hh"

#include "gui/common/CommonIO.hh"

#include <QtGui/QFontMetricsF>

#include <cassert>
#include <cmath>
#include <iostream>

namespace gui {
namespace classgf {

const qreal ClassLayout::m_default_margin{10.0};

ClassLayout::ClassLayout(
	const QString& name,
	const QString& name_with_namespace,
	const QRectF& bounding,
	qreal zoom_factor,
	const ItemRenderingOptions& opt,
	std::size_t function_count,
	std::size_t field_count
) :
	m_zoom_factor{zoom_factor},
	m_bounding{bounding},
	m_margin{ComputeMargin(QFontMetricsF{opt.class_name_font}, m_zoom_factor, m_bounding)},
	m_content{m_bounding.adjusted(m_margin, m_margin, -m_margin, -m_margin)},
	m_name_font{opt.class_name_font},
	m_member_font{opt.class_member_font},
	m_member_met{m_member_font},
	m_name{name},
	m_function_count{function_count},
	m_field_count{field_count}
{
	if (m_content.isEmpty())
		return;
	
	InitializeNameText(m_content.size() * m_zoom_factor, name_with_namespace);
		
	// size of the class name in item-space
	// calculate by scaling back
	auto name_isize = m_name.size() / zoom_factor;
	
	m_member_font.setPointSizeF(std::min(m_name_font.pointSizeF(), m_member_font.pointSizeF()));
	m_member_met = QFontMetricsF{m_member_font};
	
	auto member_height = m_member_met.height() / zoom_factor;
	DetermineFunctionFieldCount(name_isize.height(), member_height);
	
	// adjust vertical space between text
	auto total_height = name_isize.height() + (m_field_count + m_function_count) * member_height;
	
	// draw class name in the middle of the box if there's no other member
	auto name_yoffset = (m_field_count == 0 && m_function_count == 0) ?
		(m_content.height() - name_isize.height()) / 2 : 0.0;
	m_name_rect.setTopLeft({m_content.left(), m_content.top() + name_yoffset});
	m_name_rect.setSize(name_isize);
	
	if (m_field_count + m_function_count > 0)
	{
		m_member_padding =
			(m_content.height() - total_height) / (m_field_count + m_function_count); // include space between name
	}
}

qreal ClassLayout::ComputeMargin(const QFontMetricsF& name_font, qreal factor, const QRectF& bounding)
{
	auto name_height = name_font.height()/factor;
	
	// assume content has 1 line
	auto remain_height = bounding.height() - name_height;
	return remain_height > name_height ? m_default_margin/factor : std::max(remain_height/2, 0.0);
}

QRectF ClassLayout::ContentRect() const
{
	return m_content;
}

void ClassLayout::InitializeNameText(const QSizeF& content, const QString& name_with_namespace)
{
	assert(content.width() > 0);
	assert(content.height() > 0);
	
	// if there is enough space, show the namespace as well
	if (content.width() > m_name.size().width() * 2)
		m_name = QStaticText{
			QFontMetricsF{m_name_font}.elidedText(
				name_with_namespace,
				Qt::ElideLeft,
				content.width()
			)
		};
	
	for (auto trial = 0 ; trial < 5 ; trial++)
	{
		QTextOption ops{Qt::AlignCenter};
		ops.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
		m_name.setTextOption(ops);
		
		m_name.setTextFormat(Qt::PlainText);
		m_name.setTextWidth(content.width());
		m_name.prepare({}, m_name_font);
		
		// try to fit in box
		auto size = m_name.size();
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
		auto new_size = m_name_font.pointSizeF() * font_factor;
		
		if (font_factor < 1.0 && new_size > 0)
			m_name_font.setPointSizeF(m_name_font.pointSizeF() * font_factor);
		else
			break;
	}
}

QRectF ClassLayout::MemberRect(std::size_t index) const
{
	auto member_height = m_member_met.height()/m_zoom_factor;
	
	return QRectF{
		m_content.left(),
		m_name_rect.bottom() + m_member_padding + index * (m_member_padding + member_height),
		m_content.width(),
		member_height
	};
}

QLineF ClassLayout::Separator() const
{
	auto first_field = MemberRect(m_function_count);
	return QLineF{
		QPointF{m_bounding.left(), first_field.top() - m_member_padding/2},
		QPointF{m_bounding.right(), first_field.top() - m_member_padding/2}
	};
}

void ClassLayout::DetermineFunctionFieldCount(qreal name_height, qreal member_height)
{
	// total number of function and fields that can be rendered
	auto total_rows = static_cast<std::size_t>((m_content.height() - name_height) / member_height);
	
	// determine how to distribute the space between function and fields
	if (m_content.height() > name_height && total_rows > 0)
	{
		m_function_count = std::min(m_function_count, total_rows);
		m_field_count    = std::min(m_field_count,    total_rows);
		
		if (m_field_count <= total_rows / 2 && m_function_count > total_rows / 2)
			m_function_count = std::min(total_rows - m_field_count, m_function_count);
		else if (m_function_count <= total_rows / 2 && m_field_count > total_rows / 2)
			m_field_count = std::min(total_rows - m_function_count, m_field_count);
		else if (m_field_count > total_rows / 2 && m_function_count > total_rows / 2)
		{
			m_field_count    = total_rows / 2;
			m_function_count = total_rows - m_field_count;
		}
		
		assert(m_function_count <= total_rows);
		assert(m_field_count <= total_rows);
		assert(m_function_count + m_field_count <= total_rows);
	}
		
		// no space to render any function or fields
	else
		m_field_count = m_function_count = 0;
}

std::size_t ClassLayout::FunctionCount() const
{
	return m_function_count;
}

std::size_t ClassLayout::FieldCount() const
{
	return m_field_count;
}

QRectF ClassLayout::NameRect() const
{
	return m_name_rect;
}

const QStaticText& ClassLayout::Name() const
{
	return m_name;
}

QRectF ClassLayout::Header() const
{
	return m_function_count + m_field_count == 0 ? m_bounding : QRectF{
		m_bounding.topLeft(),
		QPointF{m_bounding.right(), m_name_rect.bottom() + m_member_padding/2}
	};
}

QRectF ClassLayout::Bounding() const
{
	return m_bounding;
}

qreal ClassLayout::MemberPadding() const
{
	return m_member_padding;
}

const QFont& ClassLayout::NameFont() const
{
	return m_name_font;
}

const QFont& ClassLayout::MemberFont() const
{
	return m_member_font;
}

const QFontMetricsF& ClassLayout::MemberMetrics() const
{
	return m_member_met;
}
	
}} // end of namespace
