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
#include "gui/common/CommonIO.hh"

#include <QtGui/QtGui>
#include <QtGui/QDragEnterEvent>
#include <QtCore/QMimeData>
#include <QtSvg/QSvgGenerator>
#include <QtGui/QGuiApplication>

#include <QDebug>

#include <sstream>
#include <iostream>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolTip>

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
	
	setContextMenuPolicy(Qt::DefaultContextMenu);
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
		m_model->AddEntity(usr, scene_pos);
	
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
			// item bounding rectangle in scene coordinates
			auto top_left     = mapFromScene(item->mapToScene(item->boundingRect().topLeft()));
			auto bottom_right = mapFromScene(item->mapToScene(item->boundingRect().bottomRight()));
			
			// ensure to use floating point arithmetic in calculation the zoom ratio
			qreal width  = bottom_right.x() - top_left.x();
			qreal height = bottom_right.y() - top_left.y();
			
			// calculate the zoom ratio such that the item's width or height in viewport coordinate
			// to match the viewport
			m_zoom *= std::min(
				viewport()->rect().width()   / width,
				viewport()->rect().height()  / height
			);
			
			setTransformationAnchor(QGraphicsView::AnchorViewCenter);
			setTransform(QTransform{}.scale(m_zoom, m_zoom));
			
			// point the item in the center of the viewport. after zooming, the viewport should
			// be large enough to view the whole item
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

void ClassView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu;
	
	// try to fill the clicked item in the whole viewport
	if (auto item = ClassAt(event->pos()))
	{
		connect(menu.addAction("Delete"), &QAction::triggered, [item, this]
		{
			m_model->DeleteItem(item);
		});
	}
	connect(menu.addAction("Copy"), &QAction::triggered, this, &ClassView::Copy);
	connect(menu.addAction("Paste"), &QAction::triggered, this, &ClassView::Paste);
	menu.exec(event->globalPos());
	event->accept();
}

QTransform ClassView::Transform() const
{
	return viewportTransform();
}

bool ClassView::event(QEvent *e)
{
	if (e->type() == QEvent::ToolTip)
	{
		if (auto help = dynamic_cast<QHelpEvent*>(e))
		{
			if (auto item = ClassAt(help->pos()))
			{
				auto tooltip = item->Tooltip(m_setting, m_zoom, item->mapFromScene(mapToScene(help->pos())));
				QToolTip::showText(help->globalPos(), tooltip);
				return true;
			}
		}
	}
	
	return QGraphicsView::event(e);
}

ClassItem *ClassView::ClassAt(const QPoint& pos)
{
	for (auto&& anitem : items(pos))
	{
		if (auto item = dynamic_cast<ClassItem *>(anitem))
			return item;
	}
	return nullptr;
}

void ClassView::Copy()
{
	QGuiApplication::clipboard()->setMimeData(
		m_model->CopySelection().release(),
		QClipboard::Clipboard
	);
}

void ClassView::Paste()
{
	auto data = QGuiApplication::clipboard()->mimeData(QClipboard::Clipboard);
	if (data)
		m_model->Paste(data);
}
	
}} // end of namespace
