/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "Type.hh"

#include "Cursor.hh"
#include "XStr.hh"

#include <ostream>
#include <clang-c/Index.h>

namespace libclx {

Type::Type(CXType type) :
	m_type{type}
{
}

std::string Type::Spelling() const
{
	return XStr{::clang_getTypeSpelling(m_type)}.Str();
}

std::ostream& operator<<(std::ostream& os, const Type& t)
{
	return os << t.Spelling() << ": " << " (" << t.KindSpelling() << ")";
}

Cursor Type::Declaration() const
{
	return {::clang_getTypeDeclaration(m_type)};
}

CXTypeKind Type::Kind() const
{
	return m_type.kind;
}

std::string Type::KindSpelling(CXTypeKind kind)
{
	return XStr{::clang_getTypeKindSpelling(kind)}.Str();
}

std::string Type::KindSpelling() const
{
	return KindSpelling(m_type.kind);
}

Type Type::ClassType() const
{
	return {::clang_Type_getClassType(m_type)};
}

int Type::NumTemplateArguments() const
{
	return ::clang_Type_getNumTemplateArguments(m_type);
}

Type Type::TemplateArgument(int idx) const
{
	return {::clang_Type_getTemplateArgumentAsType(m_type, idx)};
}

boost::iterator_range<Type::template_argument_iterator> Type::TemplateArguments() const
{
	auto end_idx = ::clang_Type_getNumTemplateArguments(m_type);
	
	template_argument_iterator
		begin{0, &m_type},
		end{end_idx == -1 ? 0U : static_cast<unsigned>(end_idx), &m_type};
	
	return {begin, end};
}

Type Type::PointeeType() const
{
	return {::clang_getPointeeType(m_type)};
}

void Type::template_argument_iterator::increment()
{
	m_idx++;
}

bool Type::template_argument_iterator::equal(const Type::template_argument_iterator& other) const
{
	return m_parent == other.m_parent && m_idx == other.m_idx;
}

Type Type::template_argument_iterator::dereference() const
{
	assert(m_parent);
	return {::clang_Type_getTemplateArgumentAsType(*m_parent, m_idx)};
}

void Type::template_argument_iterator::decrement()
{
	m_idx--;
}

void Type::template_argument_iterator::advance(std::ptrdiff_t n)
{
	m_idx += n;
}

std::ptrdiff_t Type::template_argument_iterator::distance_to(const Type::template_argument_iterator& other) const
{
	return other.m_idx - m_idx;
}
	
} // end of namespace
