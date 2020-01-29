#define _GNU_SOURCE
#include "color.c"
#include "icons.c"

#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
	char name[256];
	char rel_name[256];
	size_t size;
	char* icon;
	color_t color;
	time_t modify;
} File;

void usage(void);
void getSize(short*, short*);
void printFile(File*, struct stat*, char*);
void toLower(const char[256], char[256]);
int sortFile(const struct dirent**, const struct dirent**);
void humanReadableSize(const uint64_t, char*);
uint8_t getColNum(const uint64_t, const short);
static int b_all = false, b_long = false, b_human = false, b_color = true,
		   b_reverse = false;
char* dir = NULL;

int main(int argc, char** argv)
{
	uint64_t total_file_size = 0ULL;
	uint64_t total_file_width = 4ULL;	 // Initial TAB
	dir = calloc(256, sizeof(*dir));
	strcpy(dir, ".");
	// TODO -l option, long listing
	// TODO finish File struct which wraps dirent and stat / lstat(), modify time stuff,
	// owners...
	// TODO Kibi-byte for sizes. Think of a better way to determine 'KGB' rather than an
	// intense ternary
	// TODO add reverse sorting, just sort normally then reverse probably.
	// TODO Column division

	struct dirent** dirs;
	int n_of_dirs;
	short t_width, t_height;
	getSize(&t_width, &t_height);

	/* Flags:
		all = show dotfiles except for '.' & '..'

		long = long listing, ownership, permissions, modify time, size

		human_readable = Shorten sizes to Metric system, 'KGBM'.

		color = by default on, colorful version. Maybe set it in color struct? Or functions
		for color so it returns an empty string
		// TODO add no color option

		reverse = reverse printing order, currently -r but for Recursive Tree printing in
		the future maybe change to -R
	*/

	// Get the options and set the necessary flags
	{
		opterr = 0;
		int opt;
		static struct option long_options[] = {{"all", no_argument, &b_all, 1},
											   {"long", no_argument, &b_long, 1},
											   {"human", no_argument, &b_human, 1},
											   {"no-color", no_argument, &b_color, 0},
											   {"version", no_argument, 0, 'v'},
											   {"help", no_argument, 0, 'H'},
											   {"usage", no_argument, 0, 'u'},
											   {"reverse", no_argument, 0, 'r'},
											   {0, 0, 0, 0}};
		while(true)
		{
			int opt_index = 0;
			opt = getopt_long(argc, argv, "alhCHvur", long_options, &opt_index);
			if(opt == -1)
				break;

			switch(opt)
			{
				case 'a': b_all = 1; break;
				case 'l': b_long = 1; break;
				case 'h': b_human = 1; break;
				case 'C': b_color = 0; break;
				case 'H':
				case 'v':
				case 'u': usage(); break;
				case 'r': b_reverse = 1; break;
				case 0:
					if(long_options[opt_index].flag != 0)
						break;
					printf("option %s\n", long_options[opt_index].name);
					break;
				case '?': break;
				default: abort();
			}
		}
	}

	if(optind < argc)
		strcpy(dir, argv[optind++]);

	n_of_dirs = scandir(dir, &dirs, NULL, sortFile);
	if(n_of_dirs <= 0)
	{
		if(errno == ENOENT)
		{
			fprintf(stderr, "\t\033[1;31mDirectory Not Found.\033[m\n");
			return 1;
		}
		else if(errno == ENOTDIR)
		{
			struct stat status;
			stat(dir, &status);
			File file;
			strcpy(file.name, dir);
			file.icon = getIcon(file.name, 0);
			char* output_buffer = malloc(256 * sizeof(*output_buffer));
			printFile(&file, &status, output_buffer);
			printf("%s\n", output_buffer);
			free(output_buffer);
		}
		return 0;
	}

	File* v_dirs = calloc(n_of_dirs, sizeof(File));
	uint16_t num_of_files = 0U;

	{
		// Unnecessary scope
		uint8_t dotflag = 0U;
		while(n_of_dirs--)
		{
			char* name = malloc(256 * sizeof(*name));
			strcpy(name, dirs[n_of_dirs]->d_name);

			// Ignore Current and parent dirs.
			if(dotflag <= 2 && (!strcmp(name, ".") || !strcmp(name, "..")))
			{
				++dotflag;
				free(name);
				free(dirs[n_of_dirs]);
				continue;
			}

			if(b_all)
				strcpy(v_dirs[num_of_files++].name, name);
			else
			{
				if(*name == '.')
				{
					free(name);
					free(dirs[n_of_dirs]);
					continue;
				}
				else
				{
					strcpy(v_dirs[num_of_files++].name, name);
				}
			}
			free(name);
			free(dirs[n_of_dirs]);
		}
		free(dirs);
	}

	{
		// Resize file array. Even if -a isn't given we iterate over every file, .dotfiles
		// included.
		File* temp = realloc(v_dirs, num_of_files * sizeof(File));
		if(temp != NULL)
			v_dirs = temp;
		else
		{
			char* buff = malloc(24 * sizeof(*buff));
			sgetRGB(buff, 229, 195, 38);
			fprintf(stderr, "\t%sNothing to show here...\n%s", buff, RESET);
			free(buff);
			free(temp);
			free(dir);
			return 0;
		}
	}

	for(int i = num_of_files - 1; i >= 0; --i)
	{
		char* name = malloc(256 * sizeof(char));
		strcpy(name, dir);
		strcat(name, "/");
		strcat(name, v_dirs[i].name);
		// We add `dir`+/ so if we do cls .., it'll work properly. Using relative paths
		// I might've change directories, but whatever.

		struct stat status;
		lstat(name, &status);	 // lstat doesn't follow links, stat does.

		File file;
		strcpy(file.rel_name, name);
		strcpy(file.name, v_dirs[i].name);
		file.size = status.st_size;
		total_file_size += status.st_size;
		file.icon = getIcon(file.name, S_ISDIR(status.st_mode));
		free(name);
		char* output_buffer = malloc(256 * sizeof(*output_buffer));
		printFile(&file, &status, output_buffer);
		total_file_width +=
			4 +	   // 4 because the initial space and the minimum between 2
				   // strings is 4 spaces / 1 tab
			strlen(file.name) +
			3UL;	// +3 because icon + space after icon and the classification (*, /, ' ')
		/* strcpy(file.name, output_buffer); */
		strcpy(v_dirs[i].name, output_buffer);
		// Decide what to do here!
		printf("\t%s\n", output_buffer);

		free(output_buffer);
	}
	printf("\n");

	const uint8_t num_of_cols = getColNum(total_file_width, t_width);
	if(num_of_cols == 1)	// Can fit in one column
		for(int index = 0; index < num_of_files; ++index)
		{
			int rindex = num_of_files - 1 - index;
			printf("    %s", v_dirs[rindex].name);
		}
	else	// More than one row FUCK NAMING, it's not num_of_cols!!!! TODO FIXME
	{
		for(int index = 0; index < num_of_files; ++index)
		{
			int rindex = num_of_files - 1 - index;
			if(index % num_of_cols == 0)
				printf("    %s", v_dirs[rindex].name);
		}
	}
	printf("\n");

	printf("Total_Len: %lu\nTerm_Width: %d\nNo of cols: %u\n",
		   total_file_width,
		   t_width,
		   num_of_cols);
	free(v_dirs);
	free(dir);
	return 0;
}

// Function Definitions

void usage(void)
{
	printf("Usage: `cls [OPTIONS] [PATH?]`\tOrdering doesn't matter\n");
	printf("\n\t-a, --all\t\tShow files which start with . (dotfiles). Doesn't print '.' "
		   "& '..'\n");
	printf("\t-h, --human\t\tPrint file sizes in a human readable format (1024B=1K)\n");
	printf("\t-l, --long\t\tUse a long listing format. Permissions, ownership, size, "
		   "modify date\n");
	printf("\t-v, -H, -u, --version, --usage, --help:\tPrint this screen\n");
	printf("\nVersion:0.1.0\nonurorkunkader1999@gmail.com\nonurorkunkader@hotmail.com\n");
	exit(1);
}

void getSize(short* w, short* h)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*w = size.ws_col;
	*h = size.ws_row;
}

void printFile(File* file, struct stat* status, char* buff)
{
	switch(status->st_mode & S_IFMT)
	{
		struct stat t_stat;
		case S_IFDIR: strcpy(file->color.str, COLOR_DIR); break;
		case S_IFLNK:
			if(stat(file->rel_name, &t_stat) == -1)
			{
				sgetRGB(file->color.str, 255, 144, 16);
				break;
			}
			if((t_stat.st_mode & S_IFMT) == S_IFDIR)
			{
				sgetRGB(file->color.str, 111, 255, 164);
				if(file->icon == DEFAULT_FILE)
					file->icon = getIcon(file->name, 1);
			}
			else if((t_stat.st_mode & S_IFMT) == S_IFREG)
			{
				sgetRGB(file->color.str, 232, 255, 32);
				if(file->icon == DEFAULT_FILE)
					file->icon = getIcon(file->name, 0);
			}
			break;

		default: strcpy(file->color.str, COLOR_FILE);
	}

	sprintf(buff,
			"%s%s%s%c%s",
			file->color.str,
			file->icon,
			file->name,
			S_ISDIR(status->st_mode) ? '/' : (S_ISLNK(status->st_mode) ? '*' : ' '),
			RESET);
}

char lower(const char c)
{
	if(c >= 'A' && c <= 'Z')
		return (c + 32);
	return c;
}

// Fuck different Locales right? UTF-8 who?!
void toLower(const char src[256], char dest[256])
{
	uint8_t i = 0U;
	do
	{
		dest[i++] = lower(*(src));
	} while(*(src++) != '\0');
}

// I really want to change the "else" to sort 'Links -> Dirs' with other dirs.
int sortFile(const struct dirent** fir, const struct dirent** sec)
{
	// 4 for dirs, 8 for reg
	if((*fir)->d_type == 4 && ((*sec)->d_type == 8 || (*sec)->d_type == 10))
		return -1;
	else if(((*fir)->d_type == 8 || (*fir)->d_type == 10) && (*sec)->d_type == 4)
		return 1;
	else
	{
		char temp1[256];
		char temp2[256];
		toLower((*fir)->d_name, temp1);
		toLower((*sec)->d_name, temp2);
		return strverscmp(temp1, temp2);
	}
}

void humanReadableSize(uint64_t size, char* dest)
{
	const char exts[] = {'B', 'K', 'M', 'G'};
	uint8_t i = 0U;
	while(size > 1000UL)
	{
		++i;
		size /= 1000UL;
	}
	sprintf(dest, "%lu%c", size, exts[i]);
}

uint8_t getColNum(const uint64_t total_strlen, const short term_width)
{
	return 1U + (total_strlen / (1 + term_width));
}

