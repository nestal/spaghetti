/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/7/17.
//

#include "ClassView.hh"
#include "gui/common/MimeType.hh"

#include <QtGui/QtGui>
#include <QtGui/QDragEnterEvent>
#include <QtCore/QMimeData>
#include <QtGui/QGuiApplication>
#include <QtWidgets/QAbstractItemView>

#include <QDebug>

#include <sstream>

namespace gui {
namespace classgf {

ClassView::ClassView(ClassModel *model, QWidget *parent) :
	QGraphicsView{model->Scene(), parent},
	m_model{model}
{
	setAcceptDrops(true);
	setDragMode(QGraphicsView::RubberBandDrag);
	
	m_setting.class_name_font = m_setting.class_member_font = font();
	m_setting.class_name_font.setBold(true);
	
//	setViewport(new QOpenGLWidget);
}

void ClassView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(mime::usr))
		event->acceptProposedAction();
}

void ClassView::dropEvent(QDropEvent *event)
{
	auto scene_pos = mapToScene(event->pos());
	std::istringstream usrs{event->mimeData()->data(mime::usr).toStdString()};
	
	std::string usr;
	while (usrs >> usr)
		emit DropEntity(usr, scene_pos);
	
	event->acceptProposedAction();
}

void ClassView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(mime::usr))
		event->acceptProposedAction();
}

ClassModel *ClassView::Model()
{
	return m_model;
}

QWidget *ClassView::Widget()
{
	return this;
}

void ClassView::DeleteSelectedItem()
{
	m_model->DeleteSelectedItem();
}

QColor ClassView::GetLineColor() const
{
	return m_setting.line_color;
}

void ClassView::SetLineColor(QColor c)
{
	m_setting.line_color = c;
}

QColor ClassView::GetClassBoxColor() const
{
	return m_setting.class_box_color;
}

QFont ClassView::GetClassNameFont() const
{
	return m_setting.class_name_font;
}

QFont ClassView::GetClassMemberFont() const
{
	return m_setting.class_member_font;
}

void ClassView::SetClassBoxColor(QColor c)
{
	m_setting.class_box_color = c;
}

void ClassView::SetClassNameFont(QFont f)
{
	m_setting.class_name_font = f;
}

void ClassView::SetClassMemberFont(QFont f)
{
	m_setting.class_member_font = f;
}

const classgf::Setting& ClassView::Setting() const
{
	return m_setting;
}

void ClassView::wheelEvent(QWheelEvent *event)
{
	if ( QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		static const auto zoom_factor_base = 1.0015;
		m_zoom *= qPow(zoom_factor_base, event->delta());
		
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		setTransform(QTransform{}.scale(m_zoom, m_zoom));
	}
}

qreal ClassView::ZoomFactor() const
{
	return m_zoom;
}

void ClassView::ResetZoom(qreal val)
{
	m_zoom = val;
}
	
}} // end of namespace
