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
#include "Viewport.hh"

#include "ClassModel.hh"
#include "ItemRenderingOptions.hh"

#include <QtWidgets/QAction>

namespace gui {
namespace classgf {

class ClassView : public QGraphicsView, public ViewBase, public Viewport
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
	
	void Delete() override;
	
	QColor GetLineColor() const;
	QColor GetClassBoxColor() const;
	QFont GetClassNameFont() const;
	QFont GetClassMemberFont() const;
	void SetLineColor(QColor c);
	void SetClassBoxColor(QColor c) ;
	void SetClassNameFont(QFont f) ;
	void SetClassMemberFont(QFont f) ;

	const classgf::ItemRenderingOptions& Setting() const override;
	qreal ZoomFactor() const override;
	void ResetZoom() override;

	QTransform Transform() const override;

	void Copy() override;
	void Paste() override;
	
signals:
	void DropEntity(const std::string& id, const QPointF& pos);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
		
	void wheelEvent(QWheelEvent *event) override;
	
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void contextMenuEvent(QContextMenuEvent *event) override;
	
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	
	bool event(QEvent *e) override;
	
private:
	void Pan(QPointF delta);
	ClassItem* ClassAt(const QPoint& pos);

private:
	ClassModel *m_model{};
	
	// rendering options
	classgf::ItemRenderingOptions m_setting;
	
	// zoom parameters
	qreal m_zoom{1.0};
	
	// previous mouse position
	QPoint m_last_pos;
};
	
}} // end of namespace
