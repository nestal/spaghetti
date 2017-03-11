/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/19/17.
//

#pragma once

#include <QtWidgets/QTabWidget>

#include "ModelViewFwd.hh"
#include "common/ViewBase.hh"
#include <boost/iterator/iterator_facade.hpp>

namespace gui {

class Document;

/**
 * \brief A tab widget that contain widgets of type ViewBase
 *
 * This class is consider the "view" of the Document class. It is the
 * overall view of the whole project model.
 */
class ViewSet : public QTabWidget
{
public:
	using QTabWidget::QTabWidget;
	
	void Setup(Document& doc);
	
	class iterator : public boost::iterator_facade<
		iterator,
		ViewBase*,
		boost::forward_traversal_tag,
		ViewBase*
	>
	{
	public:
		iterator() = default;
		iterator(ViewSet *parent, int idx) : m_idx{idx}, m_parent{parent} {}
		
		QWidget* Widget() const;
		
	private:
		friend class boost::iterator_core_access;
		
		void increment();
		bool equal(const iterator& other) const;
		
		ViewBase* dereference() const;
		
	private:
		int     m_idx{};
		ViewSet *m_parent{};
	};
	
	iterator begin();
	iterator end();

	void OnDelete();
	void CloseAllTabs();
	void CloseTab(int tab);
	
	void ViewCode(const std::string& filename, unsigned line, unsigned column);
	void ResetZoom();
	void Copy();

private:
	void NewClassDiagramView(classgf::ClassModel *model);
	void NewSourceView(sourcevw::SourceModel *model);
	void OnRenameTab(int idx);
	
private:
	Document    *m_doc{};
};
	
} // end of namespace
