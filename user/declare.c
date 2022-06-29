#include "lib.h"

int flag[256];

void declare(char *name, char *value, u_int mode) {
    int r;
    u_int envid = syscall_getfaid(syscall_getfaid(syscall_getenvid()));
    if ((r = syscall_env_var(envid, name, value, 2, mode)) < 0) { // set
        if (r == -E_ENV_VAR_NOT_FOUND) {
            if (syscall_env_var(envid, name, value, 0, mode) < 0)  // create
                fwritef(1, "declare: too many vars\n");
			else fwritef(1, "declare: %s=%s\n", name, value);
        } else if (r == -E_ENV_VAR_RDONLY){
            fwritef(1, "declare: [%s] is readonly\n", name);
        }
    } else fwritef(1, "declare: %s=%s\n", name, value);
}

void list_var() {
    char buf[51200];
    u_int envid = syscall_getfaid(syscall_getfaid(syscall_getenvid()));
    if (syscall_env_var(envid, 0, buf, 4, 0) < 0) {    // get list
        fwritef(1, "declare: none of vars\n");
    } else {
        fwritef(1, "%s", buf);
    }
}

void usage(void) {
	fwritef(1, "usage: declare [-xr] [NAME [=VALUE]]\n");
	exit();
}

void umain(int argc, char **argv) {
    int i;
    ARGBEGIN
    {
        default:
            usage();
        case 'r':
        case 'x':
            flag[(u_char) ARGC()]++;
        break;
    }
    ARGEND

    u_int mode = 0;
    if (flag['r']) mode |= VAR_RDONLY;
    if (flag['x']) mode |= VAR_ENVIRON;

    char *value = argv[1];
    if (value[0] == '=') value++;

    if (argc == 0) list_var();
    else if (argc == 1) declare(argv[0], "", mode);
    else if (argc == 2) declare(argv[0], value, mode);
}
