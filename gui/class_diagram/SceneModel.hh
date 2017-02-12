/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#pragma once

#include <QObject>

#include <memory>

class QGraphicsScene;
class QGraphicsView;
class QPointF;

namespace codebase {
class DataType;
}

namespace gui {
namespace class_diagram {

class SceneModel : public QObject
{
public:
	SceneModel(QObject *parent);
	~SceneModel();
	
	void AttachView(QGraphicsView *view);
	
	void Clear();
	
	void AddDataType(const codebase::DataType& type, const QPointF& pos);
	
public:
	std::unique_ptr<QGraphicsScene> m_scene;
};

}} // end of namespace
