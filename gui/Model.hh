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

#include "UMLClassItem.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <boost/optional.hpp>

#include <memory>

class QGraphicsScene;
class QGraphicsView;

namespace gui {

class Model : public QObject
{
public:
	Model(QObject *parent);
	~Model() = default;

	void AttachView(QGraphicsView *view);
	void Parse(const QString& file);
	
	EntityModel* ClassModel();
	
private:
	// order is important here, since m_scene depends on m_repo.
	// m_scene contains CommitItem, which contains Commits. It must be destroyed
	// before the Repository is destroyed.
	codebase::CodeBase              m_codebase;
	std::unique_ptr<QGraphicsScene> m_scene;

	EntityModel m_class_model;
	
	struct ByUSR {};
	
	using ClassDB = boost::multi_index_container<
		UMLClassItem*,
		boost::multi_index::indexed_by<
			
			// hash by ID
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<ByUSR>,
				boost::multi_index::const_mem_fun<
					UMLClassItem,
					const std::string&,
					&UMLClassItem::ID
				>
			>
		>
	>;
	
	ClassDB m_classes;
};
	
} // end of namespace
