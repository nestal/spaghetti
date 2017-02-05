/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#pragma once

#include <clang-c/Index.h>

#include <string>

namespace libclx {

class SourceLocation
{
public:
	SourceLocation();
	SourceLocation(CXSourceLocation loc);
	SourceLocation(SourceLocation&&) = default;
	SourceLocation(const SourceLocation&) = default;
	SourceLocation& operator=(const SourceLocation&) = default;
	SourceLocation& operator=(SourceLocation&&) = default;
	
	bool operator==(const SourceLocation& rhs) const;
	bool operator!=(const SourceLocation& rhs) const;
	
	void Get(std::string& file, unsigned& line, unsigned& column, unsigned& offset) const;
	bool IsFromMainFile() const;
	bool IsFromSystemHeader() const;
	
	struct Hash
	{
		std::size_t operator()(const SourceLocation& loc) const;
	};
	friend class hash;
	friend class TranslationUnit;
	friend class SourceRange;

private:
	CXSourceLocation m_loc;
};

class SourceRange
{
public:
	SourceRange();
	SourceRange(CXSourceRange range);
	SourceRange(const SourceLocation& start, const SourceLocation& end);
	
	bool operator==(const SourceRange& rhs) const;
	bool operator!=(const SourceRange& rhs) const;

	SourceLocation Start() const;
	SourceLocation End() const;
	
	friend class TranslationUnit;
	
private:
	CXSourceRange m_range;
};
	
} // end of namespace
