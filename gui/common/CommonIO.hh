/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/8/17.
//

#pragma once

class QTransform;
class QPointF;
class QRectF;
class QSizeF;

#include <iosfwd>

std::ostream& operator<<(std::ostream& os, const QTransform& mat);
std::ostream& operator<<(std::ostream& os, const QPointF& pt);
std::ostream& operator<<(std::ostream& os, const QRectF& rect);
std::ostream& operator<<(std::ostream& os, const QSizeF& rect);
