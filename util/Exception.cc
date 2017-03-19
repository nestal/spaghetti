/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/19/17.
//

#include "Exception.hh"

#include <boost/exception/diagnostic_information.hpp>

namespace util {


const char *Exception::what() const noexcept
{
	return boost::diagnostic_information_what(*this, true);
}

} // end of namespace
