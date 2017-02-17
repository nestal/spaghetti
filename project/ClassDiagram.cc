/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/18/17.
//

#include "ClassDiagram.hh"

namespace project {


ClassDiagram::ClassDiagram(const std::string& name) : m_name{name}
{
}

void ClassDiagram::AddClass(const std::string& id, const QPointF& pos)
{
	m_classes.push_back(Class{id, pos});
}

boost::iterator_range<ClassDiagram::class_iterator> ClassDiagram::Classes() const
{
	return {m_classes.begin(), m_classes.end()};
}
	
} // end of namespace
