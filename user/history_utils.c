#include "lib.h"

void history_init() {
	//if (create("/.history", FTYPE_REG) < 0)
		//user_panic("history_init failed!\n");
	create("/.history", FTYPE_REG);
}

void history_save(char *s) {
	int fd = open("/.history", O_RDWR | O_CREAT | O_APPEND);
	if (fd < 0) user_panic("history_save open /.history failed!\n");
	fwritef(fd, s);
	fwritef(fd, "\n");
	close(fd);
}

int history_read(char (*cmd)[MAXHISTSIZE]) {
	int fd = open("/.history", O_RDONLY);
	char buf[MAXHISTSIZE*128];
	if (fd < 0) user_panic("history_read open /.history failed!\n");
	if (read(fd, buf, (long)(sizeof buf)) < 0)
		user_panic("history_read read /.history failed!\n");
	close(fd);

	int hist_size = 0, pos = 0;
	while (buf[pos]) {
		int cmd_size = 0;
		while (buf[pos] && buf[pos] != '\n')
			cmd[hist_size][cmd_size++] = buf[pos++];
		if (!buf[pos]) break;
		pos++;	// skip the '\n'
		hist_size++;
	}
	return hist_size;
}
