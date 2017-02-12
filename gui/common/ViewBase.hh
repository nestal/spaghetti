/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#pragma once

namespace gui {
namespace common {

class ModelBase;

class ViewBase
{
public:
	virtual ModelBase* Model() = 0;
};
	
}} // end of namespace
