#include "colour.c"
#include "icons.c"

#include <dirent.h>
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

void usage(void);
void getSize(short*, short*);

static int b_all = false, b_long = false, b_human = false, b_color = true,
		   b_reverse = false;

typedef struct
{
	char* name;
	size_t size;
	char* icon;
	color_t color;
	time_t modify;
} File;

int main(int argc, char** argv)
{
	// TODO finish File struct which wraps dirent and stat / lstat(), modify time stuff,
	// owners...
	// TODO add an external Color class
	// TODO Terminal checks, 256 Colors.
	// TODO Kibi-byte for sizes. Think of a better way to determine 'KGB' rather than an
	// intense ternary
	// TODO -l option, long listing
	// TODO Figure out icons, just a simple map should be fine, or maybe an array but that
	// wouldn't work nvm...
	// TODO Write your own sort function that checks the file type, not just alphabetically

	struct dirent** dirs;
	char* dir = ".";
	int n_of_dirs;
	short t_width, t_height;
	getSize(&t_width, &t_height);

	/* Flags:
		all = show dotfiles except for '.' & '..'

		long = long listing, ownership, permissions, modify time, size

		human_readable = Shorten sizes to Metric system, 'KGBM'.

		color = by default on, colorful version. Maybe set it in color struct? Or functions
		for color so it returns an empty string

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
				case 'v':
				case 'H':
				case 'u': usage(); break;

				case 0:
					if(long_options[opt_index].flag != 0)
						break;
					printf("option %s\n", long_options[opt_index].name);
				case 'a': b_all = 1; break;
				case 'l': b_long = 1; break;
				case 'h': b_human = 1; break;
				case 'C': b_color = 0; break;
				case 'r': b_reverse = 1; break;
				case '?': break;
				default: abort();
			}
		}
	}

	if(optind < argc)
		dir = argv[optind++];

	n_of_dirs = scandir(dir, &dirs, NULL, alphasort);
	if(n_of_dirs < 0)
	{
		perror("scandir:");
		return 1;
	}

	File* v_dirs = calloc(n_of_dirs, sizeof(File));
	uint16_t num_of_files = 0U;

	while(n_of_dirs--)
	{
		char* name = dirs[n_of_dirs]->d_name;

		// Ignore Current and parent dirs.
		if(!strcmp(name, ".") || !strcmp(name, ".."))
		{
			free(dirs[n_of_dirs]);
			continue;
		}

		if(b_all)
		{
			v_dirs[num_of_files].name = name;
			++num_of_files;
		}
		else
		{
			if(name[0] == '.')
			{
				free(dirs[n_of_dirs]);
				continue;
			}
			else
			{
				v_dirs[num_of_files].name = name;
				++num_of_files;
			}
		}

		// TODO Resize the v_dirs array at the end of the while loop. maybe keep a variable
		// of the size.
		/* printf("Type: %d\n", */
		/* dirs[n_of_dirs]->d_type);	// Directory = 4, Regular File = 8, Symlink = 10 */
		free(dirs[n_of_dirs]);
	}
	free(dirs);

	v_dirs = realloc(v_dirs, num_of_files * sizeof(File));

	for(int i = num_of_files - 1; i >= 0; --i)
	{
		struct stat status;
		char* name = malloc(128);
		name = strcpy(name, dir);
		strcat(name, "/");
		strcat(name, v_dirs[i].name);
		// We add dir+/ so if we do cls .., it'll work properly.

		lstat(name, &status);	 // lstat doesn't follow links, stat does.
		free(name);
		printf("Size:%ld Mode:%u ",
			   status.st_size,
			   status.st_mode);	   // S_ISREG S_ISDIR, S_ISLNK
		printf("%s\n", v_dirs[i].name);
	}

	free(v_dirs);
	return 0;
}

void usage(void)
{
	printf("Usage: `cls [OPTIONS] [PATH?]`\tOrdering doesn't matter\n");
	printf("\n\t-a, --all\t\tShow files which start with . (dotfiles). Doesn't print '.' "
		   "& '..'\n");
	printf("\t-h, --human\t\tPrint file sizes in a humar readable format (1024B=1K)\n");
	printf("\t-l, --long\t\tUse a long listing format. Permissions, ownership, size, "
		   "modify date\n");
	printf("-v, -H, -u, --version, --usage, --help:\tPrint this screen\n");
	printf("\nVersion:0.1.0\nonurorkunkader1999@gmail.com\nonurkader@protonmail.com\n");
	exit(1);
}

void getSize(short* w, short* h)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*w = size.ws_col;
	*h = size.ws_row;
}
