#include "codebase/CodeBase.hh"
#include <iostream>

int main(int argc, char **argv)
{
	cb::CodeBase base;
	base.Parse(argv[1]);
		
	return 0;
}
