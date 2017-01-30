#include "libclangxx/Index.hh"
#include <iostream>

int main(int argc, char **argv)
{
	const char *arg[] =
	{
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"DemoStat.cc",
	};
	
	clx::Index idx;
	auto tu = idx.Parse({
		"-std=c++14",
		"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/",
		"DemoStat.cc",
	}, CXTranslationUnit_None);
	
	std::cout << "translated " << tu.Spelling() << "\n";
	tu.Visit([](CXCursor cursor, CXCursor parent)
	{
		
	});
	
	for (unsigned i = 0, n = clang_getNumDiagnostics(tu.Get()); i != n; ++i)
	{
		auto diag = ::clang_getDiagnostic(tu.Get(), i);
		auto str = ::clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
		std::cerr << ::clang_getCString(str) << "\n";
		::clang_disposeString(str);
	}
		
	return 0;
}
