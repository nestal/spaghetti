/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/19/17.
//

#include "ViewSet.hh"

#include "Document.hh"
#include "source_view/View.hh"
#include "class_diagram/View.hh"

#include <QInputDialog>
#include <QTabBar>

#include <cassert>
#include <iostream>

namespace gui {

void ViewSet::iterator::increment()
{
	assert(m_parent);
	assert(m_idx >= 0);
	m_idx++;
}

bool ViewSet::iterator::equal(const ViewSet::iterator& other) const
{
	assert(m_parent);
	assert(m_idx >= 0);
	assert(other.m_parent);
	assert(other.m_idx >= 0);
	return m_idx == other.m_idx && m_parent == other.m_parent;
}

common::ViewBase* ViewSet::iterator::dereference() const
{
	assert(m_parent);
	assert(m_idx >= 0);
	return &dynamic_cast<common::ViewBase&>(*m_parent->widget(m_idx));
}

QWidget *ViewSet::iterator::Widget() const
{
	return m_parent->widget(m_idx);
}

ViewSet::iterator ViewSet::begin()
{
	return {this, 0};
}

ViewSet::iterator ViewSet::end()
{
	return {this, count()};
}

void ViewSet::Setup(Document& doc)
{
	assert(!m_doc);
	m_doc = &doc;
	
	// close widget when user clicks the close button
	connect(this, &QTabWidget::tabCloseRequested, this, &ViewSet::CloseTab);
	
	// create the corresponding view when a model is created, either by user actions or
	// when loaded from file
	connect(m_doc, &Document::OnCreateClassDiagramView, this, &ViewSet::NewClassDiagramView);
	connect(m_doc, &Document::OnCreateSourceView,       this, &ViewSet::NewSourceView);
	connect(m_doc, &Document::OnDestroyModel, [this](project::ModelBase *model)
	{
		std::cout << "destroying model " << model << std::endl;
		
		std::vector<common::ViewBase*> to_delete;
		std::copy_if(begin(), end(),
			std::back_inserter(to_delete),
			[model](auto v){return v->Model() == model;}
		);
		
		for (auto&& view : to_delete)
		{
			assert(view->Model() == model);
			removeTab(indexOf(view->Widget()));
			
			std::cout << "destroying view " << view << std::endl;
			
			delete view;
			view = nullptr;
		}
	});
	
	// double click the tab to rename it
	connect(tabBar(), &QTabBar::tabBarDoubleClicked, this, &ViewSet::OnRenameTab);
}

void ViewSet::NewClassDiagramView(class_diagram::Model *model)
{
	std::cout << "creating model " << model << std::endl;
	
	auto view   = new class_diagram::View{model, this};
	connect(view, &class_diagram::View::DropEntity, model, &class_diagram::Model::AddEntity);
	
	auto tab = addTab(view, QString::fromStdString(model->Name()));
	
	// after adding the view to the tab widget, it will be resized to fill the whole tab
	// we can use its size to resize the scene
	model->SetRect(rect());
	
	setCurrentIndex(tab);
}

void ViewSet::NewSourceView(source_view::Model *model)
{
	auto view = new source_view::View{model, this};
	addTab(view, QString::fromStdString(model->Name()));
	setCurrentWidget(view);
	
	view->setFocus(Qt::OtherFocusReason);
}

void ViewSet::CloseTab(int tab)
{
	auto w = widget(tab);
	removeTab(tab);
	
	if (auto view = dynamic_cast<common::ViewBase*>(w))
	{
		// destroy the view
		auto model = view->Model();
		delete w;

		// see if any more views are viewing this model
		// if no, destroy the model as no one is using it
		auto it = std::find_if(begin(), end(), [model](auto v){return v->Model() == model;});
		if (it == end())
			m_doc->RemoveModel(model);
	}
}

void ViewSet::CloseAllTabs()
{
	while (count() > 0)
		CloseTab(0);
}

void ViewSet::OnRenameTab(int idx)
{
	if (auto view = dynamic_cast<common::ViewBase*>(widget(idx)))
	{
		auto model = view->Model();
		assert(model);
		
		if (model->CanRename())
		{
			bool ok;
			QString text = QInputDialog::getText(
				this, tr("Rename Tab"),
				tr("New name:"), QLineEdit::Normal,
				QString::fromStdString(model->Name()), &ok
			);
			
			if (ok && !text.isEmpty())
			{
				model->SetName(text);
				tabBar()->setTabText(idx, text);
			}
		}
	}
}

void ViewSet::ViewCode(const std::string& filename, unsigned line, unsigned column)
{
	// search for existing tab showing the file
	for (int i = 0 ; i < count() ; ++i)
	{
		auto view = dynamic_cast<source_view::View*>(widget(i));
		if (view && view->Filename() == filename)
		{
			view->GoTo(line, column);
			setCurrentWidget(view);
			view->setFocus(Qt::OtherFocusReason);
			return;
		}
	}
	
	m_doc->NewSourceView(QString::fromStdString(filename), line, column);
}
	
} // end of namespace
