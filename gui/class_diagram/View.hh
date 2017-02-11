/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/7/17.
//

#pragma once

#include <QGraphicsView>

namespace gui {
namespace class_diagram {

class View: public QGraphicsView
{
Q_OBJECT

public:
	View(QWidget *parent);

signals:
	void DropEntity(const std::string& id, const QPointF& pos);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
};
	
}} // end of namespace
