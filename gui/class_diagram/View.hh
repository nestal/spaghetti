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
#include "gui/common/ViewBase.hh"

#include "Model.hh"

namespace gui {
namespace class_diagram {

class View: public QGraphicsView, public common::ViewBase
{
Q_OBJECT

public:
	View(class_diagram::Model *model, QWidget *parent);

	class_diagram::Model* Model() override;
	QWidget* Widget() override;
	
signals:
	void DropEntity(const std::string& id, const QPointF& pos);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	
private:
	class_diagram::Model   *m_model{};
};
	
}} // end of namespace
