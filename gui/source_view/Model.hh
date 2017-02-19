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

#include "project/ModelBase.hh"
#include <libclx/SourceRange.hh>

#include <QtCore/QObject>
#include <QtCore/QString>

namespace gui {
namespace source_view {

class Model : public QObject, public project::ModelBase
{
	Q_OBJECT

public:
	Model(const QString& fname, QObject *parent);
	
	void AddEntity(const std::string& id, const QPointF& pos) override;
	void SetRect(const QRectF& rect) override;
	
	bool CanRename() const override;
	std::string Name() const override;
	void SetName(const QString& name) override;
	
	void Load(const QJsonObject& obj) override;
	QJsonObject Save() const override;
	
	ModelType Type() const override {return ModelType::source_view;}
	
	bool IsChanged() const override;
	
	void SetLocation(const QString& fname, unsigned line, unsigned column);
	unsigned Line() const;
	unsigned Column() const;

signals:
	void OnLocationChanged();
	
private:
	QString m_fname;
	unsigned m_line{};
	unsigned m_column{};
};

}} // end of namespace
