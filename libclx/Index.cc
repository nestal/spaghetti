/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 1/28/17.
//

#include "Index.hh"

#include "Cursor.hh"
#include "SourceRange.hh"
#include "Token.hh"
#include "XStr.hh"

#include <boost/functional/hash.hpp>

#include <vector>
#include <ostream>

namespace libclx {

Index::Index() :
	m_index{::clang_createIndex(0, 0)}
{
	
}

TranslationUnit Index::Parse(const std::string& filename, const std::vector<std::string>& args, unsigned options)
{
	std::vector<const char*> vargs;
	for (auto&& arg : args)
		vargs.push_back(arg.c_str());
	
	return {::clang_parseTranslationUnit(
		m_index.get(), filename.c_str(), &vargs[0], static_cast<int>(vargs.size()), 0, 0, options
	)};
}

TranslationUnit::TranslationUnit(CXTranslationUnit tu) :
	m_unit{tu}
{
}

void TranslationUnit::Reparse()
{
	::clang_reparseTranslationUnit(m_unit.get(), 0, nullptr, ::clang_defaultReparseOptions(m_unit.get()));
}

std::string TranslationUnit::Spelling() const
{
	return XStr{::clang_getTranslationUnitSpelling(m_unit.get())}.Str();
}

Cursor TranslationUnit::Root() const
{
	return Cursor{::clang_getTranslationUnitCursor(m_unit.get())};
}

boost::iterator_range<TranslationUnit::diag_iterator> TranslationUnit::Diagnostics() const
{
	return {
		diag_iterator{0, m_unit.get()},
		diag_iterator{::clang_getNumDiagnostics(m_unit.get()), m_unit.get()}
	};
}

Cursor TranslationUnit::Locate(const SourceLocation& loc) const
{
	return {::clang_getCursor(m_unit.get(), loc.m_loc)};
}

TokenSet TranslationUnit::Tokenize() const
{
	return TokenSet{m_unit.get(), Root().Extent().m_range};
}

std::string TranslationUnit::TokenSpelling(const CXToken& token) const
{
	return XStr{::clang_getTokenSpelling(m_unit.get(), token)}.Str();
}

SourceLocation TranslationUnit::TokenLocation(const CXToken& token) const
{
	return {::clang_getTokenLocation(m_unit.get(), token)};
}

void TranslationUnit::diag_iterator::increment()
{
	m_idx++;
}

bool TranslationUnit::diag_iterator::equal(const TranslationUnit::diag_iterator& other) const
{
	return m_idx == other.m_idx && m_parent == other.m_parent;
}

Diagnostic TranslationUnit::diag_iterator::dereference() const
{
	return Diagnostic{::clang_getDiagnostic(m_parent, static_cast<unsigned>(m_idx))};
}

std::string Diagnostic::Str() const
{
	return XStr{::clang_formatDiagnostic(m_diag.get(), ::clang_defaultDiagnosticDisplayOptions())}.Str();
}

} // end of namespace
