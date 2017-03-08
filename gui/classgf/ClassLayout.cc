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

#include <QFontMetricsF>
#include <QTransform>

namespace gui {
namespace classgf {

const qreal ClassLayout::m_default_margin{10.0};

ClassLayout::ClassLayout(
	const QRectF& bounding,
	const QTransform& transform,
	const ItemRenderingOptions& opt,
	int function_count,
	int field_count
) :
	m_bounding{bounding},
	m_margin{ComputeMargin(QFontMetricsF{opt.class_name_font}, transform.m11(), bounding)},
	m_content{m_bounding.adjusted(m_margin, m_margin, -m_margin, -m_margin)},
	m_function_count{function_count},
	m_field_count{field_count}
{
	
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
	
}} // end of namespace
