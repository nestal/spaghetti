/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/7/17.
//

#include "ClassDiagramView.hh"
#include "logical_view/Model.hh"

#include <QDragEnterEvent>
#include <QMimeData>

#include <sstream>
#include <iostream>

namespace gui {


ClassDiagramView::ClassDiagramView(QWidget *parent) :
	QGraphicsView{parent}
{
	setAcceptDrops(true);
}

void ClassDiagramView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(logical_view::Model::m_mime_type))
		event->acceptProposedAction();
}

void ClassDiagramView::dropEvent(QDropEvent *event)
{
	auto scene_pos = mapToScene(event->pos());
	std::istringstream usrs{event->mimeData()->data(logical_view::Model::m_mime_type).toStdString()};
	
	std::string usr;
	while (usrs >> usr)
		emit DropEntity(usr, scene_pos);
	
	event->acceptProposedAction();
}

void ClassDiagramView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(logical_view::Model::m_mime_type))
		event->acceptProposedAction();
}
	
} // end of namespace
