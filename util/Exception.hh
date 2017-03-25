/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/19/17.
//

#pragma once

#include <boost/exception/exception.hpp>
#include <exception>

namespace util {

class Exception : public virtual boost::exception, public virtual std::exception
{
public:
	const char* what() const noexcept override ;
};
	
} // end of namespace
