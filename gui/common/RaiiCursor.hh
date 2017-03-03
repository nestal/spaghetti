/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/26/17.
//

#pragma once

class QCursor;

namespace gui {

class RaiiCursor
{
public:
	RaiiCursor(const QCursor& cursor);
	~RaiiCursor();
};

} // end of namespace
