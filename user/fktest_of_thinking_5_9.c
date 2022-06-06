#include "lib.h"


void umain()
{
	int a = 0;
	int id = 0;
	struct Env *e;

	if ((id = fork()) == 0) {
		id = syscall_getenvid();
		//for (;;) {
			writef("\tthis is child: id:%d\n", id);
			envid2env(id, &e, 0);
			writef("\tchild's fd addr: %08x\n", e->)
		//}
	}

	//for (;;) {
		writef("this is father: id:%d\n", id);
	//}
}
