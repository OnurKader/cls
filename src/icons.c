#ifndef ICONS_C
#define ICONS_C
#include <stdlib.h>
#include <string.h>

struct Icon
{
	char ext[16];
	char icon[9];
};

struct Icon icons[] = {

	// Peki <3
	{"Peki", "\uf004 "},
	{"peki", "\uf004 "},
	// Programming
	{"Android", "\ue70e "},
	{"android", "\ue70e "},
	{"atom", "\ue764 "},
	{"C", "\ue61e "},
	{"C#", "\uf81a "},
	{"C++", "\ue61d "},
	{"Java", "\ue256 "},
	{"Python", "\ue235 "},
	{"c", "\ue61e "},
	{"c#", "\uf81a "},
	{"c++", "\ue61d "},
	{"cc", "\ue61d "},
	{"class", "\ue256 "},
	{"code", "\ufb0f "},
	{"cpp", "\ue61d "},
	{"cs", "\uf81a "},
	{"css", "\ue749 "},
	{"Eclipse", "\ue79e "},
	{"eclipse", "\ue79e "},
	{"h", "\ue61e "},
	{"hh", "\ue61e "},
	{"hpp", "\ue61d "},
	{"htm", "\ue60e "},
	{"html", "\ue736 "},
	{"jar", "\ue256 "},
	{"java", "\ue256 "},
	{"js", "\ue74e "},
	{"json", "\ue60b "},
	{"Makefile", "\ue615 "},
	{"makefile", "\ue615 "},
	{"make", "\ue615 "},
	{"mk", "\ue615 "},
	{"o", "\ufc0c "},
	{"out", "\ufc0c "},
	{"php", "\uf81e "},
	{"py", "\ue235 "},
	{"pyc", "\ue73c "},
	{"python", "\ue235 "},
	{"pde", "\uf288 "},
	{"sh", "\uf977"},
	{"src", "\uf121 "},
	{"script", "\uf977"},
	{"scripts", "\uf977"},
	{"ts", "\ue628 "},
	{"tags", "\uf02b "},
	{"yaml", "\ue73e "},
	{"yml", "\ue73e "},
	{"vscode", "\ufb0f "},
	{"wine", "🍷"},
	// Vim
	{"nvim", "\ue62b "},
	{"vi", "\ue62b "},
	{"vim", "\ue62b "},
	{"vimrc", "\ue62b "},
	// Shh
	{"emacs", "\uf044 "},
	// Audio
	{"mid", "\uf886 "},
	{"midi", "\uf886 "},
	{"mp3", "\ufc58 "},
	{"m4a", "\ufc58 "},
	{"mpa", "\ufc58 "},
	{"Music", "\ufc58 "},
	{"ogg", "\uf886 "},
	{"wav", "\uf886 "},
	{"wma", "\uf886 "},
	// Video
	{"avi", "\uf880 "},
	{"h264", "\uf880 "},
	{"m4v", "\uf880 "},
	{"mkv", "\uf880 "},
	{"mov", "\uf880 "},
	{"mp4", "\uf880 "},
	{"mpeg", "\uf880 "},
	{"mpg", "\uf880 "},
	{"Videos", "\uf880 "},
	{"webm", "\ufa66"},
	{"wmv", "\uf880 "},
	// Image
	{"bmp", "\uf7e8 "},
	{"gif", "\uf952 "},
	{"jpg", "\uf1c5 "},
	{"jpeg", "\uf1c5 "},
	{"png", "\uf1c5 "},
	{"Pictures", "\uf1c5 "},
	{"ps", "\uf7ea "},
	{"psd", "\ue7b8 "},
	{"svg", "\ufc1f "},
	{"tiff", "\ue60d "},
	{"tif", "\ue60d "},
	{"xcf", "\uf71e "},
	// System Stuff
	{"bak", "\ue615 "},
	{"bashrc", "\ue615 "},
	{"bash", "\ue615 "},
	{"bin", "\uf114 "},
	{"cfg", "\ue615 "},
	{"conf", "\ue615 "},
	{"config", "\ue615 "},
	{"dll", "\uf830 "},
	{"firefox", "\ue745 "},
	{"exe", "\ufab2"},
	{"font", "\uf031 "},
	{"fonts", "\uf031 "},
	{"inf", "\uf7fc "},
	{"info", "\uf7fc "},
	{"ico", "\uf6f2 "},
	{"icon", "\uf6f2 "},
	{"icons", "\uf6f2 "},
	{"ini", "\ue70f "},
	{"lnk", "\ufab2"},
	{"log", "\uf89d "},
	{"msi", "\ufab2"},
	{"sys", "\ue712 "},
	{"temp", "\uf651 "},
	{"tmp", "\uf651 "},
	{"zsh", "\ue615 "},
	{"zsh_history", "\uf7d9 "},
	{"zshrc", "\ue615 "},
	// Office Stuff
	{"csv", "\uf717 "},
	{"doc", "\uf72c "},
	{"docx", "\uf72c "},
	{"latex", "\ue612 "},
	{"md", "\ue609 "},
	{"odp", "\uf1c4 "},
	{"ods", "\uf1c3 "},
	{"odt", "\uf72c "},
	{"pdf", "\uf1c1 "},
	{"pps", "\uf1c4 "},
	{"ppt", "\uf1c4 "},
	{"pptx", "\uf1c4 "},
	{"rtf", "\uf15c"},
	{"tex", "\ue612 "},
	{"txt", "\uf0f6 "},
	{"xls", "\uf1c3 "},
	{"xlsx", "\uf1c3 "},
	// Archive & Compress
	{"7z", "\uf1c6 "},
	{"bz2", "\uf066 "},
	{"bzip", "\uf066 "},
	{"bzip2", "\uf066 "},
	{"deb", "\uf306 "},
	{"gz", "\uf1c6 "},
	{"pkg", "\uf487 "},
	{"rar", "\ufac3"},
	{"rpm", "\uf316 "},
	{"tar", "\uf1c6 "},
	{"z", "\uf1c6 "},
	{"zip", "\uf1c6 "},
	// Comics
	{"Comics", "\uf02d "},
	{"cbr", "\ufc98 "},
	{"cbz", "\ufc98 "},
	// Git
	{"FETCH_HEAD", "\ue708 "},
	{"HEAD", "\ue708 "},
	{"ORIG_HEAD", "\ue708 "},
	{"branches", "\ue725 "},
	{"git", "\ue5fb "},
	{"gitconfig", "\uf1d3 "},
	{"gitignore", "\uf1d3 "},
	{"hooks", "\ufbe0 "},
	// Games
	{"Minecraft", "\uf872 "},
	{"minecraft", "\uf872 "},
	{"steam", "\uf9d2"}};

//															  
const static char default_file[] = "\uf15b", default_dir[] = "\ue5fe";

char* getIcon(char* filename)
{
	// Find the last '/' if exists or find last ., go reverse and take it from there
	const unsigned long num_of_icons = (sizeof(icons) / sizeof(struct Icon));
	char ext[16];
	char* icon = calloc(9, sizeof(char));
	char* p = rindex(filename, '.');
	if(p == NULL || *p == '\0')
		p = (filename - 1);
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

	return "\uf15b ";	 // If no extension was found, return  , default file icon.
}

#endif
