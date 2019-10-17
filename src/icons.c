#ifndef ICONS_C
#define ICONS_C
#include <string.h>

struct Icon
{
	char ext[16];
	char icon[8];
};

struct Icon icons[] = {{"asd", "\u2588"}, {"cpp", "\ue61d"}};

//															  
const static char default_file[] = "\uf15b", default_dir[] = "\ue5fe";

char* getIcon(char* filename)
{
	// Find the last '/' if exists or find last ., go reverse and take it from there
	const unsigned long num_of_icons = (sizeof(icons) / sizeof(struct Icon));
	char ext[16];
	char* icon;
	char* p = rindex(filename, '.');
	unsigned short n = 0U;
	while(*(p++) != '\0')
		ext[n++] = *p;

	for(unsigned long i = 0U; i < num_of_icons; ++i)
	{
		if(!strcmp(icons[i].ext, ext))
		{
			strcpy(icon, icons[i].icon);
			return icon;
		}
	}

	return "\uf15b";	// If no extension was found, return  , default file icon.
}

#endif
