#include "lib.h"

static char *msg = "This is the NEW message of the day!\n\n";
static char *diff_msg = "This is a different massage of the day!\n\n";

void umain()
{
        int r;
        int fdnum;
        char buf[512];
        int n;

		if ((r = open("/motd", O_RDWR | O_APPEND) < 0)) { 
			user_panic("open /motd: %d\n", r); 
		}
		fdnum = r; 
		writef("open is good\n");
		if (r = fwritef(fdnum, "test append") < 0) { 
			user_panic("fwritef %d\n", r); 
		}
		close(fdnum); 
		if ((r = open("/motd", O_RDWR|O_CREAT) < 0)) { 
			user_panic("open /motd: %d\n", r); 
		}
		if ((n = read(fdnum, buf, 150)) < 0) { 
			writef("read %d\n", n);
			return ;
		}
		writef("read is good\n");
		writef("\n%s\n", buf);

		fdnum = open("/created_file", O_RDWR|O_CREAT); fwritef(fdnum, "test create"); close(fdnum); fdnum = open("/created_file", O_RDWR); read(fdnum, buf, 150); writef("read from new file: %s\n", buf);

		fdnum = open("/newmotd", O_RDWR|O_ALONE); if ((r = fork()) == 0) { n = read(fdnum, buf, 5); writef("[child] buffer is \'%s\'\n", buf); } else { n = read(fdnum, buf, 5); writef("[father] buffer is \'%s\'\n", buf); }
/*        if ((r = open("/newmotd", O_RDWR)) < 0) {
                user_panic("open /newmotd: %d", r);
        }
        fdnum = r;
        writef("open is good\n");

        if ((n = read(fdnum, buf, 511)) < 0) {
                user_panic("read /newmotd: %d", r);
        }
        if (strcmp(buf, diff_msg) != 0) {
                user_panic("read returned wrong data");
        }
        writef("read is good\n");

        if ((r = ftruncate(fdnum, 0)) < 0) {
                user_panic("ftruncate: %d", r);
        }
        seek(fdnum, 0);

        if ((r = write(fdnum, msg, strlen(msg) + 1)) < 0) {
                user_panic("write /newmotd: %d", r);
        }

        if ((r = close(fdnum)) < 0) {
                user_panic("close /newmotd: %d", r);
        }

        //read again
        if ((r = open("/newmotd", O_RDONLY)) < 0) {
                user_panic("open /newmotd: %d", r);
        }
        fdnum = r;
        writef("open again: OK\n");

        if ((n = read(fdnum, buf, 511)) < 0) {
                user_panic("read /newmotd: %d", r);
        }
        if (strcmp(buf, msg) != 0) {
                user_panic("read returned wrong data");
        }
        writef("read again: OK\n");

        if ((r = close(fdnum)) < 0) {
                user_panic("close /newmotd: %d", r);
        }
        writef("file rewrite is good\n");
	if((r = remove("/newmotd"))<0){
		user_panic("remove /newmotd: %d",r);
	}
	if((r = open("/newmotd", O_RDONLY))>=0){
		user_panic("open after remove /newmotd: %d",r);
	}
	writef("file remove: OK\n");
        while (1) {
                //writef("IDLE!");
        }*/
}


