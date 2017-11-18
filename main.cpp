/* Compile: clang++ -isystem ../libbase/headers/ main.cpp ../libbase/src/bstring.o ../libbase/src/utf8.o -o tmplfill */
#include <stdio.h>
#include <base.h>

#include "main.h"

int main(int argc, char *argv[])
{
	if (argc < 3) { help(); return 0; };
	
	BString *file = BString::load_file(argv[1]);
	
	puts(file->c_str());
	
	return 0;
}

void help()
{
	puts("Syntax:\n\ttmplfill [IN FILE] [OUT FILE] [<key1>=<val1> <key2>=<val2>...]\n");
	puts("");
	puts("...");
}
