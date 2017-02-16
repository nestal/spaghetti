/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/28/17.
//

#pragma once

#include "util/DeleteWith.hh"

#include <clang-c/Index.h>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/operators.hpp>

#include <cassert>
#include <iosfwd>
#include <vector>

namespace libclx {

// forward declarations
class Cursor;
class Index;
class SourceLocation;
class SourceRange;
class TranslationUnit;
class Type;
class Diagnostic;
class TokenSet;

class Index
{
public:
	Index();
	Index(Index&&) = default;
	
	TranslationUnit Parse(const std::string& filename, const std::vector<std::string>& args, unsigned options);
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXIndex>,
		void,
		&::clang_disposeIndex
	> m_index;
};

class Diagnostic
{
public:
	Diagnostic(CXDiagnostic diag) : m_diag{ diag } {}

	std::string Str() const;

private:
	util::DeleteWith<
		std::remove_pointer_t<CXDiagnostic >,
		void,
		&::clang_disposeDiagnostic
	> m_diag;
};

class TranslationUnit
{
public:
	TranslationUnit(CXTranslationUnit tu);
	TranslationUnit(const TranslationUnit&) = delete;
	TranslationUnit(TranslationUnit&&) = default;
	TranslationUnit& operator=(const TranslationUnit&) = delete;
	TranslationUnit& operator=(TranslationUnit&&) = default;
	
	std::string Spelling() const;
	Cursor Root() const;
	
	Cursor Locate(const SourceLocation& loc) const;
	TokenSet Tokenize() const;
	std::string TokenSpelling(const CXToken& token) const;
	SourceLocation TokenLocation(const CXToken& token) const;
	
	class diag_iterator : public boost::iterator_facade<
		diag_iterator,
		Diagnostic,
		boost::single_pass_traversal_tag,
		Diagnostic
	>
	{
	public:
		diag_iterator() = default;
		diag_iterator(std::size_t idx, CXTranslationUnit parent) :
			m_idx{idx},
			m_parent{parent}
		{}
		
	private:
		friend class boost::iterator_core_access;
		
		void increment();
		bool equal(const diag_iterator& other) const;
		
		Diagnostic dereference() const;
		
	private:
		std::size_t       m_idx{};
		CXTranslationUnit m_parent{nullptr};
	};
	
	boost::iterator_range<diag_iterator> Diagnostics() const;
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXTranslationUnit>,
		void,
		&::clang_disposeTranslationUnit
	> m_unit;
};

class Cursor
{
public:
	Cursor();
	Cursor(CXCursor cursor);
	Cursor(const Cursor&) = default;
	Cursor(Cursor&&) = default;
	Cursor& operator=(const Cursor&) = default;
	Cursor& operator=(Cursor&&) = default;
	
	bool operator==(const Cursor& rhs) const;
	bool operator!=(const Cursor& rhs) const;
	
	CXCursorKind Kind() const;
	
	explicit operator bool() const;
	
	bool IsReference() const;
	bool IsDefinition() const;
	bool IsDeclaration() const;
	
	Cursor GetDefinition() const;
	
	std::string Spelling() const ;
	std::string DisplayName() const;
	std::string USR() const;
	std::string Comment() const;
	
	SourceLocation Location() const;
	SourceRange Extent() const;
	libclx::Type Type() const;
	
	struct Hash
	{
		unsigned operator()(Cursor c) const;
	};
	
	template <typename Visitor>
	void Visit(Visitor visitor)
	{
		auto functor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult
		{
			Visitor *pv = reinterpret_cast<Visitor*>(client_data);
			
			assert(pv);
			(*pv)(Cursor{cursor}, Cursor{parent});

			return CXChildVisit_Continue;
		};
		
		::clang_visitChildren(m_cursor, functor, &visitor);
	}

private:
	CXCursor m_cursor;
};

std::ostream& operator<<(std::ostream& os, const SourceLocation& loc);

class Type
{
public:
	Type(CXType type);
	
	std::string Spelling() const;
	Cursor Declaration() const;
	
	std::string Kind() const;
	
	friend std::ostream& operator<<(std::ostream& os, const Type& t);
	
private:
	CXType m_type;
};

} // end of namespace libclx
