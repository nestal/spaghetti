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

#include "ClassItem.hh"
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
	else
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

void ClassView::mouseDoubleClickEvent(QMouseEvent *event)
{
	// try to fill the clicked item in the whole viewport
	for (auto&& anitem : items(event->pos()))
	{
		if (auto item = dynamic_cast<ClassItem *>(anitem))
		{
			//m_zoom = item->mapToScene
			auto top_left     = mapFromScene(item->mapToScene(item->boundingRect().topLeft()));
			auto bottom_right = mapFromScene(item->mapToScene(item->boundingRect().bottomRight()));
			
			auto width  = bottom_right.x() - top_left.x();
			auto height = bottom_right.y() - top_left.y();
			
			std::cout << "item is " << width << " " << viewport()->rect().width() << " wide in viewport: " << m_zoom << std::endl;
			std::cout << "item is " << height << " " << viewport()->rect().height() << " high in viewport: " << m_zoom << std::endl;
			
			m_zoom *= std::min(
				viewport()->rect().width()/static_cast<qreal>(width),
				viewport()->rect().height()/static_cast<qreal>(height)
			);
			std::cout << "new zoom is " << m_zoom << std::endl;
			
			setTransformationAnchor(QGraphicsView::AnchorViewCenter);
			setTransform(QTransform{}.scale(m_zoom, m_zoom));
			
			// after zooming, item's width in viewport coordinate should be equal to viewport width
			
			auto top_left2     = mapFromScene(item->mapToScene(item->boundingRect().topLeft()));
			auto bottom_right2 = mapFromScene(item->mapToScene(item->boundingRect().bottomRight()));
			auto width2  = bottom_right2.x() - top_left2.x();
			auto height2 = bottom_right2.y() - top_left2.y();
			
			std::cout << "item2 is " << width2 << " " << viewport()->rect().width() << " wide in viewport: " << m_zoom << std::endl;
			std::cout << "item2 is " << height2 << " " << viewport()->rect().height() << " high in viewport: " << m_zoom << std::endl;
			
/*			auto item_view = QRectF{
				item->boundingRect().topLeft(),
				QPointF{ item->boundingRect().right(), item->boundingRect().top() + viewport()->rect().height() }
			};
			
			std::cout << "item view = " << item_view.top() << " " << item_view.left() << " " << item_view.bottom() << " " << item_view.right() << std::endl;
			
			ensureVisible(QRectF{
				item->mapToScene(item->boundingRect().topLeft()),
				item->mapToScene(QPointF{item->boundingRect().right(), item->boundingRect().top() + viewport()->rect().height()})
			}, 0, 0);*/
			centerOn(item);
		}
	}
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
