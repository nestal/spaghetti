/*
	Copyright © 2016 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the SongBits
    distribution for more details.
*/

//
// Created by nestal on 2/4/17.
//

namespace codebase {

class Class;
class SourceLocation;

class TypeDB
{
public:
	virtual ~TypeDB() = default;
	virtual const Class* Find(const SourceLocation& loc) const = 0;
	virtual void Add(const Class *class_, const SourceLocation& loc) = 0;
};

} // end of namespace
