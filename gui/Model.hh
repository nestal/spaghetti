/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the Linux
    distribution for more details.
*/

//
// Created by nestal on 9/16/16.
//

#pragma once

// parent class comes first
#include <QObject>

#include "codebase/CodeBase.hh"

#include <memory>

class QGraphicsScene;
class QGraphicsView;

namespace gui {

class Model : public QObject
{
public:
	Model(QObject *parent, QGraphicsView *view);
	~Model() = default;

private:
	// order is important here, since m_scene depends on m_repo.
	// m_scene contains CommitItem, which contains Commits. It must be destroyed
	// before the Repository is destroyed.
	codebase::CodeBase              m_codebase;
	std::unique_ptr<QGraphicsScene> m_scene;
};
	
} // end of namespace
