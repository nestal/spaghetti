/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/26/17.
//

template <typename T>
struct Temp { T t; };

struct String {};

class TypedefOwner
{
public:
	using Str = Temp<String>;
	
	Str    m_str;
	Str    *m_ptstr;
	String *m_pstr;
};
