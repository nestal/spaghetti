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
#include "gui/logical_view/LogicalModel.hh"

#include <QDragEnterEvent>
#include <QMimeData>

#include <sstream>
#include <iostream>
#include <QtWidgets/QAbstractItemView>

#include <QDebug>

namespace gui {

ClassView::ClassView(ClassModel *model, QWidget *parent) :
	QGraphicsView{model->Scene(), parent},
	m_model{model}
{
	setAcceptDrops(true);
	setDragMode(QGraphicsView::RubberBandDrag);
}

void ClassView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(LogicalModel::m_mime_type))
		event->acceptProposedAction();
}

void ClassView::dropEvent(QDropEvent *event)
{
	auto scene_pos = mapToScene(event->pos());
	std::istringstream usrs{event->mimeData()->data(LogicalModel::m_mime_type).toStdString()};
	
	std::string usr;
	while (usrs >> usr)
		emit DropEntity(usr, scene_pos);
	
	event->acceptProposedAction();
}

void ClassView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(LogicalModel::m_mime_type))
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
	return m_line_color;
}

void ClassView::SetLineColor(QColor c)
{
	m_line_color = c;
}
	
} // end of namespace
