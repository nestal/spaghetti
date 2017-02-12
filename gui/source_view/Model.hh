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

#include "gui/common/ModelBase.hh"

#include <QtCore/QObject>
#include <QtCore/QString>

namespace gui {
namespace source_view {

class Model : public common::ModelBase, public QObject
{
public:
	Model(const QString& fname, QObject *parent);
	
	void AddEntity(const std::string& id, const QPointF& pos) override;
	void SetRect(const QRectF& rect) override;
	QString Name() const override;
	void SetName(const QString& name) override;
	
private:
	QString m_fname;
};

}} // end of namespace
