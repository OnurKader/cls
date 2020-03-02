#if defined(__GNUC__)
#	define _GNU_SOURCE
#endif

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

typedef struct File
{
	char name[256];
	char rel_name[256];
	size_t size;
	// TODO Set Icon size to I dunno 27 or smth
	char icon[8];
	color_t color;
	char* user_name;
	ssize_t user_name_length;
	char* group_name;
	ssize_t group_name_length;
	time_t modify;
} File;

void usage(void);
void getSize(short*, short*);
void printFile(File*, const struct stat*, char*);
void toLower(const char[256], char[256]);
void printFileNewline(File*, const struct stat*, char*);
int sortFile(const struct dirent**, const struct dirent**);
void humanReadableSize(const uint64_t, char*, const bool);
uint8_t getColNum(const uint64_t, const short);
void getUserName(File*);
void getGroupName(File*);

static int b_all = false, b_long = false, b_human = false, b_color = true,
		   b_reverse = false, b_one_line = false;
char dir[256];

int main(int argc, char** argv)
{
	uint64_t total_file_size = 0ULL;
	uint64_t total_file_width = 4ULL;	 // Initial TAB
	// 256 because the longest legal filename is 256 on *NIX?, maybe just Linux
	// haven't checked the others. I mean I'm compiling with gnu11 so yeah...
	strcpy(dir, ".");

	// TODO -l option, long listing
	// TODO finish File struct which wraps dirent and stat / lstat(), modify
	// time stuff, owners...
	// TODO add reverse sorting, just sort normally then reverse probably.
	// TODO Column division

	struct dirent** dirs;
	int n_of_dirs = 0;

	// Get the current terminal size
	// No need to check for WINCHSIZE or whatever
	short t_width, t_height;
	getSize(&t_width, &t_height);

	/* Flags:
		all = show dotfiles except for '.' & '..'

		long = long listing, ownership, permissions, modify time, size

		human_readable = Shorten sizes to Metric system, 'KGBM'.

		color = by default on, colorful version. Maybe set it in color struct?
		Or functions for color so it returns an empty string

		reverse = reverse printing order, currently -r but for Recursive Tree
	   printing in the future maybe change to -R
	*/

	// Get the options and set the necessary flags
	{
		opterr = 0;
		int opt;
		static struct option long_options[] = {{"all", no_argument, &b_all, 'a'},
											   {"long", no_argument, &b_long, 'l'},
											   {"human", no_argument, &b_human, 'h'},
											   {"no-color", no_argument, &b_color, 'C'},
											   {"version", no_argument, 0, 'v'},
											   {"help", no_argument, 0, 'H'},
											   {"usage", no_argument, 0, 'u'},
											   {"reverse", no_argument, &b_reverse, 'r'},
											   {"one-line", no_argument, &b_one_line, '1'},
											   {0, 0, 0, 0}};
		while(true)
		{
			int opt_index = 0;
			opt = getopt_long(argc, argv, "alhCHvur1", long_options, &opt_index);
			if(opt == -1)
				break;

			switch(opt)
			{
				case 'a': b_all = true; break;
				case 'l': b_long = true; break;
				case 'h': b_human = true; break;
				case 'C': b_color = false; break;
				case 'H':
				case 'v':
				case 'u': usage(); break;
				case 'r': b_reverse = true; break;
				case '1': b_one_line = true; break;
				case 0:
					if(long_options[opt_index].flag != 0)
						break;
					printf("option %s\n", long_options[opt_index].name);
					break;
				case '?': usage(); break;
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
			fprintf(stderr, "\t\033[1;31mFile or Directory not found%s\n", RESET);
			return 1;
		}
		else if(errno == ENOTDIR)
		{
			struct stat status;
			stat(dir, &status);
			File file;
			strcpy(file.name, dir);
			strcpy(file.icon, getIcon(file.name, 0));
			char* output_buffer = calloc(256, sizeof(*output_buffer));

			// Print the filename if `cls` is run on a file
			// TODO when --long is added determine what to do here
			printFileNewline(&file, &status, output_buffer);
		}
		return 0;
	}

	File* v_dirs = calloc(n_of_dirs, sizeof(File));
	uint16_t num_of_files = 0U;

	// Unnecessary scope
	{
		uint8_t dotflag = 0U;
		while(n_of_dirs--)
		{
			char name[256];
			strcpy(name, dirs[n_of_dirs]->d_name);

			// Ignore Current and parent dirs.
			if(dotflag < 2 && (!strcmp(name, ".") || !strcmp(name, "..")))
			{
				++dotflag;
				free(dirs[n_of_dirs]);
				continue;
			}

			if(b_all)
				strcpy(v_dirs[num_of_files++].name, name);
			else
			{
				if(name[0] == '.')
				{
					free(dirs[n_of_dirs]);
					continue;
				}
				else
				{
					strcpy(v_dirs[num_of_files++].name, name);
				}
			}
			free(dirs[n_of_dirs]);
		}
		free(dirs);
	}

	{
		// Resize file array. Even if -a isn't given we iterate over every file,
		// .dotfiles included.
		File* temp = realloc(v_dirs, num_of_files * sizeof(File));
		if(temp != NULL)
			v_dirs = temp;
		else
		{
			char buff[27];
			sgetRGB(buff, 229, 195, 38);
			fprintf(stderr, "\t%sNothing to show here...\n%s", buff, RESET);
			free(temp);
			return 0;
		}
	}

	for(int i = num_of_files - 1; i >= 0; --i)
	{
		char name[256];
		strcpy(name, dir);
		strcat(name, "/");
		strcat(name, v_dirs[i].name);
		// We add `dir`+/ so if we do cls .., it'll work properly. Using
		// relative paths I might've changed directories, but whatever.

		struct stat status;
		lstat(name, &status);	 // lstat doesn't follow links, stat does.

		File file;
		strcpy(file.rel_name, name);
		strcpy(file.name, v_dirs[i].name);
		file.size = status.st_size;
		total_file_size += status.st_size;
		strcpy(file.icon, getIcon(file.name, S_ISDIR(status.st_mode)));

		char output_buffer[256];
		printFile(&file, &status, output_buffer);
		total_file_width += 4 +	   // 4 because the initial space and the minimum between 2
								   // strings is 4 spaces / 1 tab
							strlen(file.name) +
							3UL;	// +3 because icon + space after icon
									// and the classification (*, /, ' ')
		/* strcpy(file.name, output_buffer); */
		/* strcpy(v_dirs[i].name, output_buffer); */
		// TODO Decide what to do here!
		if(b_one_line)
			printf("\t%s\n", output_buffer);
		else
			printf("\t%s\t", output_buffer);
	}

	if(!b_one_line)
		printf("\n");

	free(v_dirs);
	return 0;
}

// Function Definitions

void usage(void)
{
	printf("Usage: `cls [OPTIONS] [PATH?]`\tOrdering doesn't matter\n");
	printf("\n\t-a, --all\t\tShow files which start with . (dotfiles). Doesn't "
		   "print '.' "
		   "& '..'\n");
	printf("\t-h, --human\t\tPrint file sizes in a human readable format "
		   "(1024B=1K)\n");
	printf("\t-l, --long\t\tUse a long listing format. Permissions, ownership, "
		   "size, "
		   "modify date\n");
	printf("\t-v, -H, -u, --version, --usage, --help:\tPrint this screen\n");
	printf("\nVersion:0.1.1\nonurorkunkader1999@gmail.com\nonurorkunkader@"
		   "hotmail.com\n");
	exit(1);
}

void getSize(short* w, short* h)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*w = size.ws_col;
	*h = size.ws_row;
}

void printFile(File* file, const struct stat* status, char* buff)
{
	switch(status->st_mode & S_IFMT)
	{
		struct stat t_stat;
		case S_IFDIR: strcpy(file->color.str, COLOR_DIR); break;
		case S_IFLNK:
			if(stat(file->rel_name, &t_stat) == -1)
			{
				sgetRGB(file->color.str, 255, 101, 12);
				break;
			}
			if((t_stat.st_mode & S_IFMT) == S_IFDIR)
			{
				sgetRGB(file->color.str, 100, 255, 164);
				if(!strcmp(file->icon, DEFAULT_FILE))
					strcpy(file->icon, getIcon(file->name, 1));
			}
			else if((t_stat.st_mode & S_IFMT) == S_IFREG)
			{
				sgetRGB(file->color.str, 232, 255, 32);
				if(!strcmp(file->icon, DEFAULT_FILE))
					strcpy(file->icon, getIcon(file->name, 0));
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

void humanReadableSize(uint64_t size, char* dest, const bool kibi)
{
	// XXX Might not work
	uint8_t i = 0U;
	const char* exts[5];
	uint64_t divisor;
	if(kibi)
	{
		divisor = 1024ULL;
		exts[0] = "B";
		exts[1] = "KiB";
		exts[2] = "MiB";
		exts[3] = "GiB";
		exts[4] = "TiB";
	}
	else
	{
		divisor = 1000ULL;
		exts[0] = "B";
		exts[1] = "K";
		exts[2] = "M";
		exts[3] = "G";
		exts[4] = "T";
	}

	while(size > divisor)
	{
		++i;
		size /= divisor;
	}
	sprintf(dest, "%lu%s", size, exts[i]);
}

uint8_t getColNum(const uint64_t total_strlen, const short term_width)
{
	return 1U + (total_strlen / (1 + term_width));
}

void printFileNewline(File* file, const struct stat* status, char* output_buffer)
{
	printFile(file, status, output_buffer);
	printf("\t%s\n", output_buffer);
	free(output_buffer);
}

void getUserName(File* file)
{
	file->user_name_length = sysconf(_SC_GETPW_R_SIZE_MAX);
	if(file->user_name_length == -1)
		file->user_name_length = 256U;
	file->user_name = calloc(1, file->user_name_length);
	if(!file->user_name)
	{
		fprintf(stderr, "Couldn't MALLOC!\n");
		exit(1);
	}
	strcpy(file->user_name, getpwuid(geteuid())->pw_name);
}

void getGroupName(File* file)
{
	file->group_name_length = sysconf(_SC_GETGR_R_SIZE_MAX);
	if(file->group_name_length == -1)
		file->group_name_length = 256U;
	file->group_name = calloc(1, file->group_name_length);
	if(!file->group_name)
	{
		fprintf(stderr, "Couldn't MALLOC!\n");
		exit(1);
	}
	strcpy(file->group_name, getgrgid(geteuid())->gr_name);
}
