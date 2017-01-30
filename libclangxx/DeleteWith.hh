/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 1/30/17.
//

#pragma once

#include <type_traits>
#include <memory>

namespace util {

template <typename T, typename R, R (*deleter)(T*)>
struct FunctionDeleter
{
	void operator()(T *t) {(*deleter)(t);}
};

template <typename T, typename R, R (*deleter)(T*)>
using DeleteWith = std::unique_ptr<T, FunctionDeleter<T, R, deleter>>;

}