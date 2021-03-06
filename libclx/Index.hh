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
#include "util/Exception.hh"

#include <clang-c/Index.h>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/operators.hpp>
#include <boost/exception/error_info.hpp>

#include <cassert>
#include <iosfwd>
#include <vector>

/**
 * \brief A thin wrapper around libclang-c
 *
 * libclx is a thin wrapper around libclang-c. Its purpose is to simplify the usage.
 */
namespace libclx {

// forward declarations
class Cursor;
class Index;
class SourceLocation;
class TranslationUnit;
class Diagnostic;
class TokenSet;

class Index
{
public:
	using ErrorCode = boost::error_info<struct ErrorCodeTag, int>;
	
public:
	Index();
	Index(Index&&) = default;
	
	static std::string Version();
	
	TranslationUnit Parse(const std::string& filename, const std::vector<std::string>& args, unsigned options);
	void Reparse(TranslationUnit& tu);
	
private:
	util::DeleteWith<
		std::remove_pointer_t<CXIndex>,
		void,
		&::clang_disposeIndex
	> m_index;
	util::DeleteWith<
		std::remove_pointer_t<CXIndexAction>,
		void,
		&::clang_IndexAction_dispose
	> m_action;
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
	
	void Reparse();
	
	std::string Spelling() const;
	Cursor Root() const;
	
	Cursor Locate(const SourceLocation& loc) const;
	TokenSet Tokenize() const;
	std::string TokenSpelling(const CXToken& token) const;
	SourceLocation TokenLocation(const CXToken& token) const;
	
	class diag_iterator : public boost::iterator_facade<
		diag_iterator,
		Diagnostic,
		boost::forward_traversal_tag,
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

} // end of namespace libclx
