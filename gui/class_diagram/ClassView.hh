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

#include "ClassModel.hh"

namespace gui {

class ClassView: public QGraphicsView, public ViewBase
{
Q_OBJECT

public:
	ClassView(ClassModel *model, QWidget *parent);

	ClassModel* Model() override;
	QWidget* Widget() override;
	
	void DeleteSelectedItem();
	
signals:
	void DropEntity(const std::string& id, const QPointF& pos);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	
private:
	ClassModel   *m_model{};
};
	
} // end of namespace
