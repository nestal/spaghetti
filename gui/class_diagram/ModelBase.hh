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

#include <string>

class QGraphicsScene;
class QPointF;
class QRectF;
class QString;

namespace gui {
namespace class_diagram {

class ModelBase
{
public:
	virtual QGraphicsScene* Scene() = 0;
	virtual void AddEntity(const std::string& id, const QPointF& pos) = 0;
	virtual void SetRect(const QRectF& rect) = 0;
	virtual QString Name() const = 0;
	virtual void SetName(const QString& name) = 0;
};

}} // end of namespace
