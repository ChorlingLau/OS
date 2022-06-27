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
    if (path[0] != '/') fwritef(1, "./%s\n", path);
	else fwritef(1, ".%s\n", path);
    // DFS walk and print
    walk(path, 0, recursive);
}

void walk(char *path, int level, int recursive) {
    int fd;
    struct File f, f_next;
    struct File *dir;
    
    // end of the recursive
    if (recursive == 0) return;

    if ((fd = open(path, O_RDONLY)) < 0)
        user_panic("open %s: %e", path, fd);
	
	if (readn(fd, &f, (sizeof f)) != sizeof f) return;

	int last = 0;
	while (!last) {
		last = (readn(fd, &f_next, (sizeof f_next)) != sizeof f_next);
		if (!f_next.f_name[0]) last = 1;
		// writef("\"last\" in tree.c/walk: %d\n", last);
		if (f.f_name[0]) {
            dir = &f;
            printcs(' ', 4*level, 0);
			if (last) fwritef(1, "`-- %s\n", dir->f_name);
            else fwritef(1, "|-- %s\n", dir->f_name);
            
            if (dir->f_type == FTYPE_DIR) {
                char new[MAXPATHLEN] = {0};
                strcpy(new, path);
                strcat(new, "/");
                strcat(new, dir->f_name);
                walk(new, level+1, recursive-1);
            }
        }
	//	writef("f.f_name: %s, &f_next.f_name: %s\n", f.f_name, f_next.f_name);
		user_bcopy(&f_next, &f, sizeof f);
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
	// writef("tree.b start...\n");
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
		for (i=0; i<argc; i++) {
			/*char path[MAXPATHLEN] = {0};
			if (argv[i][0] != '/') {
				strcpy(path, "/");
			}
			strcat(path, argv[i]);*/
			tree(argv[i], argv[i]);
		}
	}
}
