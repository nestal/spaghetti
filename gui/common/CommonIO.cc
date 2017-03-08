/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/8/17.
//

#include "CommonIO.hh"

#include <QTransform>

std::ostream& operator<<(std::ostream& os, const QTransform& mat)
{
	return os
		<< mat.m11() << ' ' << mat.m12() << ' ' << mat.m13() << '\n'
		<< mat.m21() << ' ' << mat.m22() << ' ' << mat.m23() << '\n'
		<< mat.m31() << ' ' << mat.m32() << ' ' << mat.m33() ;
}

std::ostream& operator<<(std::ostream& os, const QPointF& pt)
{
	return os << '(' << pt.x() << ',' << pt.y() << ')';
}

std::ostream& operator<<(std::ostream& os, const QRectF& rect)
{
	return os << '{' << rect.topLeft() << ',' << rect.bottomRight() << '}';
}
