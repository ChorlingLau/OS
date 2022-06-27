#include "lib.h"

const char *CURPATH_KEY = "curpath";

void curpath_init(char *path) {
    if (syscall_env_var(CURPATH_KEY, path, 0) < 0)
        user_panic("curpath_init failed! path: %s\n", path);
}

int curpath_get(char *path) {
    return syscall_env_var(CURPATH_KEY, path, 1);
}

int curpath_set(char *path) {
    return syscall_env_var(CURPATH_KEY, path, 2);
}

int curpath_get_parent(char *path) {
    int i;
    curpath_get(path);
    if (strlen(path) == 1) return 1;

    for (i = strlen(path) - 2; path[i - 1] != '/'; i--);
    path[i] = 0;
    return 0;
}
