/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
    u_int dev_addr = 0x13000000;
    u_char status = 0;
    u_char read_value = 0;

	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		int tmp_off = offset_begin + offset;
        if (syscall_write_dev((u_int)&diskno, dev_addr + 0x10, 4) < 0)
            user_panic("ide_read err 0x10\n");
        if (syscall_write_dev((u_int)&tmp_off, dev_addr + 0x0, 4) < 0)
            user_panic("ide_read err 0x0\n");
        if (syscall_write_dev((u_int)&read_value, dev_addr + 0x20, 1) < 0)
            user_panic("ide_read err 0x20\n");

        status = 0;
        if (syscall_read_dev((u_int)&status, dev_addr + 0x30, 1) < 0)
            user_panic("ide_read err 0x30\n");
        if (!status)
            user_panic("ide_read failed!\n");

        if (syscall_read_dev((u_int)(dst + offset), dev_addr + 0x4000, 0x200) < 0)
            user_panic("ide_read err 0x4000\n");
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	u_int dev_addr = 0x13000000;
	u_char status = 0;
	u_char write_value = 1;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.
		// error occurred, then panic.
		int tmp_off = offset_begin + offset;
		if (syscall_write_dev((u_int)&diskno, dev_addr + 0x10, 4) < 0)
			user_panic("ide_write err 0x10\n");
		if (syscall_write_dev((u_int)&tmp_off, dev_addr + 0x0, 4) < 0)
			user_panic("ide_write err 0x0\n");
		if (syscall_write_dev((u_int)(src + offset), dev_addr + 0x4000, 0x200) < 0)
			user_panic("ide_write err 0x4000\n");
		if (syscall_write_dev((u_int)&write_value, dev_addr + 0x20, 1) < 0)
			user_panic("ide_write err 0x20\n");
		
		status = 0;
		if (syscall_read_dev((u_int)&status, dev_addr + 0x30, 1) < 0)
			user_panic("ide_write err 0x30\n");
		if (!status) 
			user_panic("ide_write failed!\n");
		offset += 0x200;
	}
}

int time_read() {
	int time;
	int trigger = 1;
	if (syscall_write_dev((u_int)&trigger, 0x15000000, 1) < 0)
		user_panic("get_time err 0x0\n");
	if (syscall_read_dev((u_int)&time, 0x15000010, 4) < 0)
		user_panic("get_time err 0x10\n");
	return time;
}

void raid0_write(u_int secno, void *src, u_int nsecs) {
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int now_secno = secno;
	while (offset_begin + offset < offset_end) {
		ide_write(1 + now_secno % 2, now_secno / 2, src + offset, 1);
		offset += 0x200;
		now_secno += 1;
	}
}

void raid0_read(u_int secno, void *dst, u_int nsecs) {
    int offset_begin = secno * 0x200;
    int offset_end = offset_begin + nsecs * 0x200;
    int offset = 0;
	int now_secno = secno;
    while (offset_begin + offset < offset_end) {
        ide_read(1 + now_secno % 2, now_secno / 2, dst + offset, 1);
        offset += 0x200;
		now_secno += 1;
    }
}
