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
#include "EntityModel.hh"

#include <QDragEnterEvent>
#include <QMimeData>

#include <sstream>

namespace gui {


ClassDiagramView::ClassDiagramView(QWidget *parent) :
	QGraphicsView{parent}
{
	setAcceptDrops(true);
}

void ClassDiagramView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(EntityModel::m_mime_type))
	{
		std::istringstream usrs{event->mimeData()->data(EntityModel::m_mime_type).toStdString()};
		
		std::string usr;
//		while (usrs >> usr)
//			std::cout << usr << " " << i++ << std::endl;
		
		event->acceptProposedAction();
	}
}

void ClassDiagramView::dropEvent(QDropEvent *event)
{
	event->acceptProposedAction();
}

} // end of namespace
