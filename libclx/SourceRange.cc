/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/5/17.
//

#include "SourceRange.hh"
#include "XStr.hh"

#include <boost/functional/hash.hpp>

namespace libclx {

SourceLocation::SourceLocation() :
	SourceLocation{::clang_getNullLocation()}
{
}

SourceLocation::SourceLocation(CXSourceLocation loc) :
	m_loc{loc}
{
}

void SourceLocation::Get(std::string& file, unsigned& line, unsigned& column, unsigned& offset) const
{
	CXFile xfile;
	::clang_getFileLocation(m_loc, &xfile, &line, &column, &offset);
	file = XStr{::clang_getFileName(xfile)}.Str();
}

bool SourceLocation::IsFromMainFile() const
{
	return ::clang_Location_isFromMainFile(m_loc) != 0;
}

bool SourceLocation::IsFromSystemHeader() const
{
	return ::clang_Location_isInSystemHeader(m_loc) != 0;
}

bool SourceLocation::operator==(const SourceLocation& rhs) const
{
	return ::clang_equalLocations(m_loc, rhs.m_loc) != 0;
}

bool SourceLocation::operator!=(const SourceLocation& rhs) const
{
	return !operator==(rhs);
}

std::size_t SourceLocation::Hash::operator()(const SourceLocation& loc) const
{
	CXFile file{};
	unsigned line, column, offset;
	::clang_getFileLocation(loc.m_loc, &file, &line, &column, &offset);
	
	CXFileUniqueID uid{};
	if (::clang_getFileUniqueID(file, &uid))
	{
		std::size_t seed{};
		for (auto&& data : uid.data)
			boost::hash_combine(seed, data);
		
		boost::hash_combine(seed, offset);
		return seed;
	}
	return 0;
}

SourceRange::SourceRange(CXSourceRange range) :
	m_range{range}
{
}

SourceRange::SourceRange() :
	SourceRange{::clang_getNullRange()}
{
}

SourceRange::SourceRange(const SourceLocation& start, const SourceLocation& end) :
	SourceRange{::clang_getRange(start.m_loc, end.m_loc)}
{
}

bool SourceRange::operator==(const SourceRange& rhs) const
{
	return ::clang_equalRanges(m_range, rhs.m_range) != 0;
}

bool SourceRange::operator!=(const SourceRange& rhs) const
{
	return !operator==(rhs);
}

SourceLocation SourceRange::Start() const
{
	return {::clang_getRangeStart(m_range)};
}

SourceLocation SourceRange::End() const
{
	return {::clang_getRangeEnd(m_range)};
}
	
} // end of namespace
