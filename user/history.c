#include "lib.h"

int flag[256];

void usage(void) {
    fwritef(1, "usage: history\n");
    exit();
}

void umain(int argc, char **argv) {
    int i, size;
    char cmd[MAXHISTSIZE][128] = {0};

    ARGBEGIN{
    default:
        usage();
        break;
    }ARGEND

    size = history_read(cmd);
    for (i = 0; i < size; i++) {
		char tmp[300];
		strcpy(tmp, cmd[i]);
        fwritef(1, " %03d %s\n", i, tmp);
    }

}
        
