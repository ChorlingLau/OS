#include "lib.h"

int flag[256];

void mkdir(char *path, char *perfix) {
    int r;
    struct Stat st;
    char curpath[MAXPATHLEN] = {0};

    if ((r = curpath_get(curpath)) < 0) {
        fwritef(1, "mkdir: cannot get environment var [curpath]\n");
    }

    if (path[0] == '/') {
        strcpy(curpath, path);
    } else {
        if (curpath[strlen(curpath) - 1] != '/') {
            strcat(curpath, "/");  
        }
        strcat(curpath, path);
    }

    if ((r = create(curpath, FTYPE_DIR)) < 0) {
        fwritef(1, "mkdir: directory %s already exists\n", curpath);
    } else {
        fwritef(1, "mkdir: successfully create directory %s\n", curpath);
    }
}

void usage(void) {
	fwritef(1, "usage: mkdir [OPTION]... directory...\n");
	exit();
}

void umain(int argc, char **argv)
{
	int i;

	ARGBEGIN{
	default:
		usage();
	case 'd':
	case 'F':
	case 'l':
		flag[(u_char)ARGC()]++;
		break;
	}ARGEND

	if (argc == 0)
		fwritef(1, "mkdir: missing operand");
	else {
		for (i=0; i<argc; i++)
			mkdir(argv[i], argv[i]);
	}
	fwritef(1, "\n");
}

