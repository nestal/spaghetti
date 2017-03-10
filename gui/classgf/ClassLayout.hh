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
#include <QtGui/QStaticText>
#include <QtGui/QFontMetricsF>

#include <cstddef>

class QRectF;
class QTransform;
class QLineF;
class QFontMetricsF;

namespace gui {
namespace classgf {

struct ItemRenderingOptions;

class ClassLayout
{
public:
	ClassLayout(
		const QString& name,
		const QString& name_with_namespace,
		const QRectF& bounding,
		qreal zoom_factor,
		const ItemRenderingOptions& opt,
		std::size_t function_count,
		std::size_t field_count
	);

	QRectF Bounding() const;
	QRectF NameRect() const;
	QRectF Header() const;
	QRectF ContentRect() const;
	QLineF Separator() const;

	std::size_t FunctionCount() const;
	std::size_t FieldCount() const;
	QRectF MemberRect(std::size_t index) const;

	const QFont& NameFont() const;
	const QFont& MemberFont() const;
	const QFontMetricsF& MemberMetrics() const;
	const QStaticText& Name() const;
	
private:
	static qreal ComputeMargin(const QFontMetricsF& name_font, qreal factor, const QRectF& bounding);
	void InitializeNameText(const QSizeF& content, const QString& name_with_namespace);
	void DetermineFunctionFieldCount(qreal name_height, qreal member_height);
	
private:
	qreal   m_zoom_factor;
	QRectF  m_bounding;
	qreal   m_margin;
	QRectF  m_content;
	
	QFont   m_name_font;
	QFont   m_member_font;
	QFontMetricsF m_member_met;
	
	QStaticText m_name;
	QRectF      m_name_rect;
	
	std::size_t m_function_count;
	std::size_t m_field_count;

	qreal m_member_padding{-1};
	
	static const qreal m_default_margin;
};

}} // end of namespace
