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
#include "Type.hh"
#include "XStr.hh"

#include "util/Exception.hh"

#include <boost/functional/hash.hpp>
#include <boost/exception/errinfo_file_name.hpp>
#include <boost/exception/info.hpp>

#include <vector>
#include <ostream>
#include <clang-c/Index.h>

namespace libclx {

Index::Index() :
	m_index{::clang_createIndex(0, 0)},
	m_action{::clang_IndexAction_create(m_index.get())}
{
	
}

TranslationUnit Index::Parse(const std::string& filename, const std::vector<std::string>& args, unsigned options)
{
	std::vector<const char*> vargs;
	for (auto&& arg : args)
		vargs.push_back(arg.c_str());
	
	IndexerCallbacks cb{};
//	cb.indexEntityReference = [](CXClientData, const CXIdxEntityRefInfo *ref)
//	{
//		std::cout << "ref = " << ref->referencedEntity->name << " " << ref->parentEntity->name << " " << ref->kind << " " << Cursor{ref->container->cursor}.Type() << std::endl;
//		std::cout << "ref = " << ref->referencedEntity->name << " " << ref->referencedEntity->templateKind << std::endl;
//	};
//	cb.indexDeclaration = [](CXClientData, const CXIdxDeclInfo *decl)
//	{
//		std::cout << "decl: " << decl->entityInfo->name << " " << decl->entityInfo->kind << std::endl;
//
//		if (auto class_ = ::clang_index_getCXXClassDeclInfo(decl))
//		{
//			std::cout << "has " << class_->numBases << " base classes" << std::endl;
//			for (auto i = 0U ; i < class_->numBases ; i++)
//			{
//				auto base = class_->bases[i]->base;
//				if (base)
//				{
//					assert(base->name);
//					std::cout << "\t\"" << base->name << "\" " << base->kind << " " << Cursor{base->cursor}.Type()
//					          << std::endl;
//				}
////				else
////					std::cout << "base is null for " << decl->entityInfo->name << std::endl;
//			}
//		}
//	};
	
	CXTranslationUnit tu;
	auto r = ::clang_indexSourceFile(
		m_action.get(),
		
		nullptr,
		&cb,
		sizeof(cb),
		
		CXIndexOpt_IndexImplicitTemplateInstantiations | CXIndexOpt_SuppressWarnings | CXIndexOpt_SkipParsedBodiesInSession ,
		filename.c_str(),
		&vargs[0],
		static_cast<int>(vargs.size()),
		0,
		0,
		&tu,
		options
	);
	
	if (r == 0)
		return {tu};
	else
		BOOST_THROW_EXCEPTION(util::Exception{} << boost::errinfo_file_name(filename) << ErrorCode{r});
}

std::string Index::Version()
{
	return XStr{::clang_getClangVersion()}.Str();
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
