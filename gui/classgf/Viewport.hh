/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/4/17.
//

#pragma once

#include <QtCore/QtGlobal>

namespace gui {
namespace classgf {

class Setting;

class Viewport
{
protected:
	~Viewport() = default;
	
public:
	virtual const classgf::Setting& Setting() const = 0;
	virtual qreal ZoomFactor() const = 0;
};

}} // end of namespace
