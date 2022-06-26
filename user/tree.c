#include "lib.h"

int flag[256];

void tree_start(char*, u_int);
void walk(char*, int, int);
void printcs(char, int, int);

void tree(char *path, char *prefix) {
    int r;
	struct Stat st;

	if ((r = stat(path, &st)) < 0)
		user_panic("stat %s: %e", path, r);
	tree_start(path, -1);
}

void tree_start(char *path, u_int recursive) {
    fwritef(1, ".%s", path);
    // DFS walk and print
    walk(path, 0, recursive);
}

void walk(char *path, int level, int recursive) {
    int fd;
    struct File f;
    struct File *dir;
    
    // end of the recursive
    if (recursive == 0) return;

    if ((fd = open(path, O_RDONLY)) < 0)
        user_panic("open %s: %e", path, fd);
    while (readn(fd, &f, (sizeof f)) == (sizeof f)) {
        if (f.f_name[0]) {
            dir = &f;
            printcs(' ', 4*level, 0);
            fwritef(1, "|-- %s\n", dir->f_name);
            
            if (dir->f_type == FTYPE_DIR) {
                char new[MAXPATHLEN] = {0};
                strcpy(new, path);
                strcat(new, "/");
                strcat(new, f.f_name);
                walk(new, level+1, recursive-1);
            }
        }
    }
    
}

void printcs(char c, int n, int line_end) {
    int i;
    while (n--) {
        fwritef(1, "%c", c);
    }
    if (line_end) fwritef(1, "\n");
}

void usage(void) {
	fwritef(1, "usage: tree [-dFl] [directory...]\n");
	exit();
}

void umain(int argc, char **argv) {
	writef("tree.b start...\n");
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
		tree("/", "");
	else {
		for (i=0; i<argc; i++)
			tree(argv[i], argv[i]);
	}
}
