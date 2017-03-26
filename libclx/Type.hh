/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#pragma once

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/operators.hpp>

#include <clang-c/Index.h>

#include <string>
#include <iosfwd>

namespace libclx {

class Cursor;

class Type
{
public:
	Type(CXType type);
	
	std::string Spelling() const;
	Cursor Declaration() const;
	
	std::string KindSpelling() const;
	CXTypeKind Kind() const;
	static std::string KindSpelling(CXTypeKind kind);
	
	Type ClassType() const;
	int NumTemplateArguments() const;
	Type TemplateArgument(int idx) const;
	
	class template_argument_iterator;
	
	boost::iterator_range<template_argument_iterator> TemplateArguments() const;
	
	friend std::ostream& operator<<(std::ostream& os, const Type& t);

private:
	CXType m_type;
};

class Type::template_argument_iterator : public boost::iterator_facade<
	Type::template_argument_iterator,
	Type,
	boost::random_access_traversal_tag,
	Type
>
{
public:
	template_argument_iterator() = default;
	template_argument_iterator(unsigned idx, const CXType *parent) :
		m_idx{idx},
		m_parent{parent}
	{}

private:
	friend class boost::iterator_core_access;
	
	void increment();
	void decrement();
	bool equal(const template_argument_iterator& other) const;
	
	Type dereference() const;
	void advance(std::ptrdiff_t n);
	std::ptrdiff_t distance_to(const template_argument_iterator& other) const;

private:
	unsigned        m_idx{};
	const CXType    *m_parent{};
};

} // end of namespace
