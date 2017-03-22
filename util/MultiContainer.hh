/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/22/17.
//

#pragma once

#include <iterator>

namespace util {

template<typename Base, typename... Ts>
struct MultiContainer;

template<typename Base, typename Head>
struct MultiContainer<Base, Head>
{
	virtual const Base *At(std::size_t idx) const
	{
		return &m_types.at(idx);
	}
	
	virtual Base *At(std::size_t idx)
	{
		return &m_types.at(idx);
	}
	
	void Add(Head&& entity)
	{
		m_types.push_back(std::move(entity));
	}
	
	virtual std::size_t Size() const
	{
		return m_types.size();
	}
	
	virtual std::ptrdiff_t IndexOf(const Base *base) const
	{
		auto diff = std::distance(&m_types[0], static_cast<const Head *>(base));
		if (diff >= m_types.size())
			throw std::out_of_range("oops");
		return diff;
	}
	
	std::vector<Head> m_types;
};

template<typename Base, typename Head, typename... Ts>
struct MultiContainer<Base, Head, Ts...> : public MultiContainer<Base, Ts...>
{
	const Base *At(std::size_t idx) const override
	{
		if (idx < m_types.size())
			return &m_types.at(idx);
		else
			return MultiContainer<Base, Ts...>::At(idx - m_types.size());
	}
	
	Base *At(std::size_t idx) override
	{
		if (idx < m_types.size())
			return &m_types.at(idx);
		else
			return MultiContainer<Base, Ts...>::At(idx - m_types.size());
	}
	
	void Add(Head&& entity)
	{
		m_types.push_back(std::move(entity));
	}
	
	std::size_t Size() const override
	{
		return m_types.size() + MultiContainer<Base, Ts...>::Size();
	}
	
	std::ptrdiff_t IndexOf(const Base *base) const override
	{
		auto diff = std::distance(&m_types[0], static_cast<const Head *>(base));
		if (diff < m_types.size())
			return diff;
		
		else
			return m_types.size() + MultiContainer<Base, Ts...>::IndexOf(base);
	}
	
	std::vector<Head> m_types;
};

template<typename Head, typename Base, typename... Ts>
std::vector<Head>& GetHelper(MultiContainer<Base, Head, Ts...>& vec)
{
	return vec.m_types;
}

template<typename T, typename Base, typename... Ts>
std::vector<T>& Get(MultiContainer<Base, Ts...>& vec)
{
	return GetHelper<T>(vec);
}

template<typename Base, typename T, typename... Ts>
T& Add(MultiContainer<Base, Ts...>& cond, T&& entity)
{
	auto& vec = GetHelper<T>(cond);
	vec.push_back(std::forward<T>(entity));
	return vec.back();
}

} // end of namespace
