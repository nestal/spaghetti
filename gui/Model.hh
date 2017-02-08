/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 9/16/16.
//

#pragma once

#include <QObject>

#include "EntityModel.hh"
#include "codebase/CodeBase.hh"

#include <memory>

class QGraphicsScene;
class QGraphicsView;

namespace gui {

class UMLClassItem;

class Model : public QObject
{
public:
	Model(QObject *parent);
	~Model();

	void AttachView(QGraphicsView *view);
	void Parse(const QString& file);
	
	QAbstractItemModel* ClassModel();
	
	libclx::SourceLocation LocateEntity(const QModelIndex& idx) const;
	void AddEntity(const std::string& id, const QPointF& pos);
	
private:
	// order is important here, since m_scene depends on m_repo.
	// m_scene contains CommitItem, which contains Commits. It must be destroyed
	// before the Repository is destroyed.
	codebase::CodeBase              m_codebase;
	std::unique_ptr<QGraphicsScene> m_scene;

	EntityModel m_class_model;
};
	
} // end of namespace
