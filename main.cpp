/* Compile: clang++ -isystem ../libbase/headers/ main.cpp ../libbase/src/bstring.o ../libbase/src/utf8.o -o tmplfill */
#include <stdio.h>
#include <base.h>

#include "main.h"

int main(int argc, char *argv[])
{
	if (argc < 3) { help(); return 0; };
	
	/* Read the file */
	BString *file = BString::load_file(argv[1]);
	
	if (file == NULL)
	{
		fprintf(stderr, "Error: File '%s' not found.\n", argv[1]);
		return 1;
	}
	
	/* Replace the placeholders */
	bool errors = false;

	for (int32 offset = 0; offset < file->length() && file->index_of('$', file->char_at(offset)) != -1; offset += file->index_of('$', file->char_at(offset)))
	{
		if (*(file->char_at(offset + 1)) != '(') continue;
		
		BString *key   = new BString(file->char_at(offset + 2), false, file->offset_of(')', file->char_at(offset + 2)));
		BString *value = NULL;
		
		/* Search for the key in the arguments */
		for (int i = 3; i < argc; i++)
		{
			if (key->equals(argv[i], key->size()))
			{
				value = new BString(strchr(argv[i], '=') + 1);
			}
		}
		
		if (value == NULL)
		{
			fprintf(stderr, "Error: Value for '%s' not given.\n", key->c_str());
			errors = true;
		}
		else
		{
			file->remove(file->char_at(offset), strchrnul(file->char_at(offset), ')') + 1);
			file->insert(value, offset);		// So we see that calling realloc in remove and insert changes the start address of the string.
		}
		
		delete key;
		delete value;
		offset++;	// So that we don't get stuck;
	}
	
	if (errors) { delete file; return 1; }
	
	/* Save the output */
	bool rc = file->save_file(argv[2]);
	
	if (!rc)
	{
		fprintf(stderr, "Error: Could not write file '%s'.\n", argv[2]);
		delete file;
		return 1;
	}
	
	return 0;
}

char *help_text[] = {
	"Syntax:\n\ttmplfill [IN FILE] [OUT FILE] [<key1>=<val1> <key2>=<val2>...]\n",
	NULL
};

void help()
{
	for (char **line = help_text; *line != NULL; line++)
	{
		puts(*line);
	}
}
