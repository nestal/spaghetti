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
#include <iostream>

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

const classgf::ItemRenderingOptions& ClassView::Setting() const
{
	return m_setting;
}

void ClassView::wheelEvent(QWheelEvent *event)
{
	if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		static const auto zoom_factor_base = 1.0005;
		m_zoom *= qPow(zoom_factor_base, event->delta());
		
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		setTransform(QTransform{}.scale(m_zoom, m_zoom));
				
		event->accept();
	}

	QGraphicsView::wheelEvent(event);
}

qreal ClassView::ZoomFactor() const
{
	return m_zoom;
}

void ClassView::ResetZoom()
{
	m_zoom = 1.0;
	
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	setTransform(QTransform{}.scale(m_zoom, m_zoom));
}

void ClassView::mouseMoveEvent(QMouseEvent *event)
{
	if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier && event->buttons() & Qt::LeftButton)
	{
		Pan(mapToScene(event->pos()) - mapToScene(m_last_pos));
		event->accept();
	}
	else
		QGraphicsView::mouseMoveEvent(event);
	
	m_last_pos = event->pos();
}

void ClassView::mousePressEvent(QMouseEvent *event)
{
	if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton)
	{
		setCursor(Qt::CursorShape::ClosedHandCursor);
		m_last_pos = event->pos();
		event->accept();
	}
	else
		QGraphicsView::mousePressEvent(event);
}

void ClassView::mouseReleaseEvent(QMouseEvent *event)
{
	if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton)
		setCursor(Qt::CursorShape::OpenHandCursor);

	QGraphicsView::mouseReleaseEvent(event);
}

void ClassView::Pan(QPointF delta)
{
	delta *= m_zoom;
	
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QPoint new_center{
		static_cast<int>(viewport()->rect().width() / 2 - delta.x()),
		static_cast<int>(viewport()->rect().height() / 2 - delta.y())
	};
	centerOn(mapToScene(new_center));
	
	// For zooming to anchor from the view center.
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

void ClassView::keyPressEvent(QKeyEvent *event)
{
	std::cout << "pressed! " << std::hex << event->key() << std::endl;
	if (event->key() == Qt::Key_Control)
		setCursor(Qt::CursorShape::OpenHandCursor);
	
	QGraphicsView::keyPressEvent(event);
}

void ClassView::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
		setCursor(Qt::CursorShape::ArrowCursor);
	
	QGraphicsView::keyReleaseEvent(event);
}

}} // end of namespace
