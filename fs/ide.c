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

int raid4_valid(u_int diskno) {
	//writef("raid4_valid start...\n");	
	int tmp_off = 0;
	u_char status = 0;
	u_char read_value = 0;
	u_int dev_addr = 0x13000000;
	if (syscall_write_dev((u_int)&diskno, dev_addr + 0x10, 4) < 0)
		user_panic("valid err 0x10\n");
	if (syscall_write_dev((u_int)&tmp_off, dev_addr + 0x0, 4) < 0)
        user_panic("valid err 0x0\n");
    if (syscall_write_dev((u_int)&read_value, dev_addr + 0x20, 1) < 0)
        user_panic("valid err 0x20\n");
    if (syscall_read_dev((u_int)&status, dev_addr + 0x30, 1) < 0)
        user_panic("valid err 0x30\n");
	
	if (status) return 1;
	else return 0;
}

 int raid4_write(u_int blockno, void *src) {
	// writef("raid4_write start...\n");
	int bad[5] = {0};
	u_char content[8][512] = {0};
	int diskno, secno;
	int offset_begin = blockno * 2 * 0x200;
	int offset_end = offset_begin + 8 * 0x200;
	int offset = 0;
	for (diskno = 1; diskno <= 5; diskno++) {
		bad[diskno-1] = 1 - raid4_valid(diskno);
	}
	while (offset_begin + offset < offset_end) {
//		writef("write...\n");
		diskno = (offset / 0x200) % 4 + 1;
//		writef("diskno::::%d\n", diskno);
		secno = blockno * 2 + offset / 0x200 / 4;
//		writef("secno::::%d\n", secno);
		if (bad[diskno - 1]) {
			offset += 0x200;
			continue;
		}
		ide_write(diskno, secno, src + offset, 1);
//		writef("%d\n", *(int *)(src + offset));
//		user_bzero(content[offset / 0x200], 0x200);
		user_bcopy((void *)src + offset, (void *)content[offset / 0x200], 0x200);
//		writef("%d\n", *(int *)(content[offset / 0x200]));
		offset += 0x200;
	}
	
	u_char p[2][512] = {0};
	int cnt = 0, i = 0;
	for (i = 0; i < 512; i++) {
//		writef("%d %d %d %d\n", content[0][i], content[1][i], content[2][i], content[3][i]);
		p[0][i] = content[0][i] ^ content[1][i] ^ content[2][i] ^ content[3][i];
		p[1][i] = content[4][i] ^ content[5][i] ^ content[6][i] ^ content[7][i];
	}
//	writef("%d, %d\n", p[0][3], p[1][3]);
	if (!bad[4]) { 
		ide_write(5, blockno*2 + 0, p[0], 1);
	    ide_write(5, blockno*2 + 1, p[1], 1);
	}
	
	for (i = 0; i < 5; i++) {
		cnt += bad[i];
	}
	return cnt;
 }

int raid4_read(u_int blockno, void *dst) {
	int bad[5] = {0}, badno = -1;
    u_char content[8][512] = {0}, p[2][512] = {0};;
    int diskno, secno;
    int offset_begin = blockno * 2 * 0x200;
    int offset_end = offset_begin + BY2PG;
    int offset = 0;
    for (diskno = 1; diskno <= 5; diskno++) {
        bad[diskno-1] = 1 - raid4_valid(diskno);
		if (bad[diskno-1]) badno = diskno;
    }
	int i, cnt = 0;
	for (i = 0; i < 5; i++) {
        cnt += bad[i];
    }
    if (cnt > 1) return cnt;
	while (offset_begin + offset < offset_end) {
        diskno = (offset / 0x200) % 4 + 1;
        secno = blockno * 2 + offset / 0x200 / 4;
        if (bad[diskno - 1]) {
	//		user_bzero(content[offset / 0x200], 0x200);
			offset += 0x200;
			continue;
		}
        ide_read(diskno, secno, dst + offset, 1);
  //      user_bzero((void *)content[offset / 0x200], 0x200);
        user_bcopy(dst+offset, (void *)content[offset / 0x200], 0x200);
		offset += 0x200;
    }
	if (cnt == 0) {
		ide_read(5, blockno*2, p[0], 1);
        ide_read(5, blockno*2+1, p[1], 1);
		for (i = 0; i < 512; i++) {
	//		writef("%d %d\n", p[0][i], p[1][i]);
			if (p[0][i] != content[0][i] ^ content[1][i] ^ content[2][i] ^ content[3][i] ||
				p[1][i] != content[4][i] ^ content[5][i] ^ content[6][i] ^ content[7][i]) {
				return -1;
			}
		}
		return 0;
	}

	if (cnt == 1) {
		if (badno == 5) {
			return 1;
		} else {
			offset = (diskno-1)*0x200;
			char tmp[512] = {0};
			for (i = 0; i < 512; i++) {
			
			}
			return 1;
		}
	}
}
