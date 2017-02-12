/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/7/17.
//

#include "View.hh"
#include "gui/common/ModelBase.hh"
#include "gui/logical_view/Model.hh"

#include <QDragEnterEvent>
#include <QMimeData>

#include <sstream>
#include <iostream>

namespace gui {
namespace class_diagram {

View::View(QGraphicsScene *scene, QWidget *parent) :
	QGraphicsView{scene, parent}
{
	setAcceptDrops(true);
}

void View::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(logical_view::Model::m_mime_type))
		event->acceptProposedAction();
}

void View::dropEvent(QDropEvent *event)
{
	auto scene_pos = mapToScene(event->pos());
	std::istringstream usrs{event->mimeData()->data(logical_view::Model::m_mime_type).toStdString()};
	
	std::string usr;
	while (usrs >> usr)
		emit DropEntity(usr, scene_pos);
	
	event->acceptProposedAction();
}

void View::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(logical_view::Model::m_mime_type))
		event->acceptProposedAction();
}
	
}} // end of namespace
