#include "lib.h"

int flag[256];

void usage() {
	fwritef(1, "usage: test_hang [&]\n");
	exit();
}

#define TIME 1500
int a[TIME][TIME];
void umain(int argc, char **argv) {
	int i, j;
	ARGBEGIN
    {
        default:
            usage();
        break;
    }
    ARGEND
	for (i = 0; i < TIME; i++) {
		for (j = 0; j < TIME; j++) {
			a[j][i] = i+j;
		}
		if (i % 500 == 0) fwritef(1, BROWN(test_hang is storing\n));
	}
	fwritef(1, BROWN(test_hang finishes storing\n));
	for (i = 0; i < TIME; i++) {
        for (j = 0; j < TIME; j++) {
            if (i % 555 == 0 && j % 1111 == 0) 
				fwritef(1, BROWN(test_hang is running: %06d\n), a[j][i]);
        }
    }
}
