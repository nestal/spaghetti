#include "codebase/CodeBase.hh"
#include <iostream>

int main(int argc, char **argv)
{
	cb::CodeBase base;
	base.Parse(argv[1]);
	
	for (auto&& class_ : base)
		std::cout << class_ << std::endl;
		
	return 0;
}
