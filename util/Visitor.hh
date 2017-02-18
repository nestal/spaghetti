/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/18/17.
//

#pragma once

namespace util {

template <typename... Args>
struct Visitor;

template <typename Arg, typename... OtherArgs>
struct Visitor<Arg, OtherArgs...> : Visitor<OtherArgs...>
{
	template <typename T, typename F>
	void Visit(T&& t, F&& func)
	{
		if (auto at = dynamic_cast<Arg*>(&t))
			func(*at);
		else
			Visitor<OtherArgs...>::Visit(std::forward<T>(t), std::forward<F>(func));
	}
};

template <>
struct Visitor<>
{
	template <typename T, typename F>
	void Visit(T&& t, F&& func)
	{
		func(t);
	}
};

template <typename V, typename T>
void Visit(V&& v, T&& t)
{
	v.Visit(std::forward<T>(t), std::forward<V>(v));
}

} // end of namespace
