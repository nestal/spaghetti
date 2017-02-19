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
#include "gui/logical_view/Model.hh"

#include <QDragEnterEvent>
#include <QMimeData>

#include <sstream>
#include <iostream>
#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QAbstractScrollArea>

namespace gui {
namespace class_diagram {

View::View(class_diagram::Model *model, QWidget *parent) :
	QGraphicsView{model->Scene(), parent},
	m_model{model}
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

class_diagram::Model *View::Model()
{
	return m_model;
}

QWidget *View::Widget()
{
	return this;
}

void View::DeleteSelectedItem()
{
	m_model->DeleteSelectedItem();
}
	
}} // end of namespace
