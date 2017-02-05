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

#include <QtCore/QFile>

#include <iostream>

namespace gui {

void SourceView::Open(const libclx::SourceLocation& file)
{
	std::cout << "opening " << file << std::endl;
	std::string filename;
	unsigned line, column, offset;
	file.Get(filename, line, column, offset);
	
	QFile qfile{QString::fromStdString(filename)};
	if (qfile.open(QIODevice::ReadOnly | QIODevice::Text))
		setPlainText(QString{qfile.readAll()});
}

} // end of namespace
