/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/18/17.
//

#pragma once

#include <QtCore/QPointF>

#include <boost/range.hpp>

#include <string>
#include <vector>

namespace project {

class ClassDiagram
{
public:
	struct Class
	{
		std::string id;
		QPointF     pos;
	};
	using class_iterator = std::vector<Class>::const_iterator;
	
public:
	ClassDiagram(const std::string& name);

	void AddClass(const std::string& id, const QPointF& pos);
	
	boost::iterator_range<class_iterator> Classes() const;
	
private:
	std::string m_name;
	std::vector<Class> m_classes;
};

} // end of namespace