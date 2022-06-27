#include "lib.h"

int flag[256];

void unset(char *name) {
    int r;
    if ((r = syscall_env_var(name, 0, 3, 0))) { // unset
        if (r == -E_ENV_VAR_NOT_FOUND)
            fwritef(1, "unset: [%s] doesn't exists\n", name);
        else if (r == -E_ENV_VAR_RDONLY)
            fwritef(1, "unset: [%s] is readonly\n", name);
    }
}

void usage(void) {
	fwritef(1, "usage: unset [name ...]\n");
	exit();
}

void umain(int argc, char **argv) {
    int i;
    ARGBEGIN
    {
        default:
            usage();
        break;
    }
    ARGEND

    if (argc == 0) {
        return;
    } else {
        for (i = 0; i < argc; i++)
            unset(argv[i]);
    }
}