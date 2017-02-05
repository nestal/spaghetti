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

#include <QTextEdit>

namespace libclx {
class SourceLocation;
}

namespace gui {

class SourceView : public QTextEdit
{
public:
	using QTextEdit::QTextEdit;
	
	void Open(const libclx::SourceLocation& location);
};
	
} // end of namespace
