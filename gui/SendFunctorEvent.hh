/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/6/17.
//

#pragma once

#include <QCoreApplication>

namespace gui {

template <typename F>
void SendFunctorEvent(QObject *receiver, F&& func)
{
	struct Event : public QEvent
	{
		using Functor = typename std::decay<F>::type;
		
		Event(Functor&& fun) : QEvent{QEvent::None}, m_func{std::move(fun)} {}
		Event(const Functor& fun) : QEvent{QEvent::None}, m_func{fun} {}
		~Event()
		{
			try {m_func();} catch (...) {}
		}
		
		Functor m_func;
	};
	
	QCoreApplication::postEvent(receiver, new Event{std::forward<F>(func)});
}

} // end of namespace
