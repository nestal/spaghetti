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
#include <QTextEdit>

#include "Model.hh"
#include <thread>

namespace libclx {
class SourceLocation;
class TranslationUnit;
}

namespace gui {
namespace source_view {

class View : public QTextEdit, public common::ViewBase
{
public:
	View(source_view::Model *model, QWidget *parent);
	~View();
	
	void Open(const libclx::SourceLocation& location);
	void GoTo(unsigned line, unsigned column);
	
	const std::string& Filename() const;
	
	source_view::Model* Model() override;

private:
	class HighlightEvent;
	
	void Parse(unsigned line, unsigned column);
	void Highlight(unsigned line, unsigned column, std::size_t stride, const QColor& colour);
	
	std::thread m_worker;
	std::string m_filename;
	
	source_view::Model *m_model{};
};
	
}} // end of namespace
