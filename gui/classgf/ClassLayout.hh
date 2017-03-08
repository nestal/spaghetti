/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/8/17.
//

#pragma once

#include <QtCore/QRectF>

class QRectF;
class QTransform;
class QLineF;
class QFontMetricsF;

namespace gui {
namespace classgf {

class ItemRenderingOptions;

class ClassLayout
{
public:
	ClassLayout(
		const QRectF& bounding,
		const QTransform& transform,
		const ItemRenderingOptions& opt,
		int function_count,
		int field_count
	);
	
	QRectF NameRect() const;
	QRectF ContentRect() const;
	QLineF NameLine() const;
	QRectF Separator() const;
	QRectF FunctionRect(int index) const;
	QRectF FieldRect(int index) const;
			
private:
	static qreal ComputeMargin(const QFontMetricsF& name_font, qreal factor, const QRectF& bounding);
	
private:
	QRectF  m_bounding;
	qreal   m_margin;
	QRectF  m_content;
	
	int m_function_count;
	int m_field_count;
	
	static const qreal m_default_margin;
};

}} // end of namespace
