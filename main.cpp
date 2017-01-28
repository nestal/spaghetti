#include <clang-c/Index.h>
#include <iostream>

int main(int argc, char **argv)
{
	const char *arg[] =
		{
			"-std=c++14",
			"DemoStat.cc",
			"-I", "/usr/lib/gcc/x86_64-redhat-linux/6.3.1/include/"
		};
	
	auto idx = ::clang_createIndex(0, 0);
	auto tu  = ::clang_parseTranslationUnit(idx, 0, arg, sizeof(arg)/sizeof(arg[0]), 0, 0, CXTranslationUnit_None);
	
	for (unsigned i = 0, n = clang_getNumDiagnostics(tu); i != n; ++i)
	{
		auto diag = ::clang_getDiagnostic(tu, i);
		auto str = ::clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
		std::cerr << ::clang_getCString(str) << "\n";
		::clang_disposeString(str);
	}
		
	::clang_disposeTranslationUnit(tu);
	::clang_disposeIndex(idx);
	
	return 0;
}
