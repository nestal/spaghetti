/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "SourceView.hh"

#include "libclx/Index.hh"
#include "libclx/SourceRange.hh"
#include "libclx/Token.hh"

#include <QtCore/QFile>

namespace gui {

void SourceView::Open(const libclx::SourceLocation& file)
{
	std::string filename;
	unsigned line, column, offset;
	file.Get(filename, line, column, offset);
	
	QFile qfile{QString::fromStdString(filename)};
	if (qfile.open(QIODevice::ReadOnly))
		setText(qfile.readAll());
	
 
}

} // end of namespace
