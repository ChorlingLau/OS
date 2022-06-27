#include "lib.h"

int flag[256];

void touch(char *path, char *perfix) {
	int fd;
	char curpath[MAXPATHLEN] = {0};
	if (curpath_get(curpath) < 0) {
		fwritef(1, "touch: cannot get environment var [curpath]\n");
	}

	if (path[0] != '/') {
		if (curpath[strlen(curpath) - 1] != '/')
            strcat(curpath, "/");
        strcat(curpath, path);
	}

	if (create(curpath, FTYPE_REG) < 0) {
        fwritef(1, "file %s already exists!\n", curpath);
        return;
    }
    fwritef(1, "successfully create file %s\n", curpath);
}

void usage() {
	fwritef(1, "usage: touch [file...]\n");
	exit();
}

void umain(int argc, char **argv) {
	int i;
    ARGBEGIN
    {
        default:
            usage();
        case 'd':
        case 'F':
        case 'l':
            flag[(u_char) ARGC()]++;
        break;
    }
    ARGEND
    if (argc == 0) {
        return;
    } else {
        for (i = 0; i < argc; i++)
            touch(argv[i], argv[i]);
    }
}
