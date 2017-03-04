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
#include "Setting.hh"

namespace gui {
namespace classgf {

class ClassView : public QGraphicsView, public ViewBase
{
	Q_OBJECT
	Q_PROPERTY(QColor lineColor     READ GetLineColor     WRITE SetLineColor     DESIGNABLE true)
	Q_PROPERTY(QColor classBoxColor READ GetClassBoxColor WRITE SetClassBoxColor DESIGNABLE true)
	Q_PROPERTY(QFont  classNameFont READ GetClassNameFont WRITE SetClassNameFont DESIGNABLE true)
	Q_PROPERTY(QFont  classMemberFont READ GetClassMemberFont WRITE SetClassMemberFont DESIGNABLE true)

public:
	ClassView(ClassModel *model, QWidget *parent);
	
	ClassModel *Model() override;
	QWidget *Widget() override;
	
	void DeleteSelectedItem();
	
	QColor GetLineColor() const;
	QColor GetClassBoxColor() const;
	QFont GetClassNameFont() const;
	QFont GetClassMemberFont() const;
	void SetLineColor(QColor c);
	void SetClassBoxColor(QColor c) ;
	void SetClassNameFont(QFont f) ;
	void SetClassMemberFont(QFont f) ;

	const classgf::Setting& Setting() const;
	qreal ZoomFactor() const;
	void ResetZoom() override;
	
signals:
	void DropEntity(const std::string& id, const QPointF& pos);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
		
	void wheelEvent(QWheelEvent *event) override;
	
private:
	ClassModel *m_model{};
	
	// rendering options
	classgf::Setting m_setting;
	
	// zoom parameters
	qreal m_zoom{1.0};
};
	
}} // end of namespace
