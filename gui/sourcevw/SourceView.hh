/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#pragma once

#include "gui/common/ViewBase.hh"
#include <QtWidgets/QPlainTextEdit>

#include "SourceModel.hh"
#include <thread>
#include <atomic>

namespace gui {
namespace sourcevw {

class SourceView : public QPlainTextEdit, public ViewBase
{
public:
	SourceView(SourceModel *model, QWidget *parent);
	~SourceView();
	
	void Open(const std::string& fname, unsigned line, unsigned column);
	void GoTo(unsigned line, unsigned column);
	
	const std::string& Filename() const;
	
	SourceModel* Model() override;
	QWidget* Widget() override;

private:
	void Parse(unsigned line, unsigned column);
	void Highlight(unsigned line, unsigned column, std::size_t stride, const QColor& colour);
	
	std::thread m_worker;
	std::string m_filename;
	
	SourceModel *m_model{};
	
	QTextCursor m_highlight;
};

}} // end of namespace
