/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

#define IDE_PADDR (0x13000000)

// #define DEBUG_IDERW
// #define DEBUG_SHOWMEM

#ifdef DEBUG_SHOWMEM
void debug_mem_print(void *start, u_int size) {
    int i;
    writef("@%x: ", start);
    for (i = 0; i < size; i++) {
        writef("%02x", (u_int)(*((u_char*)start + i)));
    }
    writef(" -EOL\n");
}
#endif

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
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
    
    int r;
    u_int tmpword;
    u_char tmpbyte;
#ifdef DEBUG_IDERW
    writef("ide_read: diskno=%d, secno=%d, nsecs=%d\n", diskno, secno, nsecs);
#endif
	while (offset_begin + offset < offset_end) {
        // Your code here
        // error occurred, then panic.

        /* 
         * 1. *0x0010 = IDE ID(diskno)
         * 2. *0x0000 = offset
         * 3. *0x0020 = 0 (read)
         * 4. read *0x0030
         * 5. read 512 bytes from 0x4000
         */
        tmpword = diskno;
        if ((r = syscall_write_dev(&tmpword, IDE_PADDR + 0x0010, 4)) < 0) {
            user_panic("ide_read failed.(1 sel diskno)");
        }
        
        tmpword = offset + offset_begin; // BUG FIX!
        if ((r = syscall_write_dev(&tmpword, IDE_PADDR + 0x0000, 4)) < 0) {
            user_panic("ide_read failed.(2 sel offset)");
        }

        tmpbyte = 0;
        if ((r = syscall_write_dev(&tmpbyte, IDE_PADDR + 0x0020, 1)) < 0) {
            user_panic("ide_read failed.(3 set read)");
        }

        if ((r = syscall_read_dev(&tmpbyte, IDE_PADDR + 0x0030, 1)) < 0) {
            user_panic("ide_read failed.(4 fetch status)");
        }
        
        if (tmpbyte == 0) {
            user_panic("ide_read failed.(5 check status)");
        }

        if ((r = syscall_read_dev(dst + offset, IDE_PADDR + 0x4000, BY2SECT)) < 0) {
            user_panic("ide_read failed.(6 read buffer)");
        }
        // debug_mem_print(dst + offset, BY2SECT);
        offset += BY2SECT;
	}
#ifdef DEBUG_IDERW
    writef("ide_read: read ");
#ifdef DEBUG_SHOWMEM
    debug_mem_print(dst, nsecs * BY2SECT);
#endif
#endif
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
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
#ifdef DEBUG_IDERW
    writef("ide_write: diskno=%d, secno=%d, nsecs=%d\n", diskno, secno, nsecs);
#endif
    // DO NOT DELETE WRITEF !!!
	// writef("diskno: %d\n", diskno);
#ifdef DEBUG_IDERW 
    writef("ide_write: will write ");
#ifdef DEBUG_SHOWMEM
    debug_mem_print(src, nsecs * BY2SECT);
#endif
#endif
    
    u_int tmpword;
    u_char tmpbyte;
    int r;
	while (offset_begin + offset < offset_end) {
	    // copy data from source array to disk buffer.
        // if error occur, then panic.
        
        /* 
         * 1. *0x0010 = IDE ID(diskno)
         * 2. *0x0000 = offset
         * 3. write 512 bytes to 0x4000
         * 4. *0x0020 = 1 (write)
         * 5. read *0x0030, check status
         */
        tmpword = diskno;
        if ((r = syscall_write_dev(&tmpword, IDE_PADDR + 0x0010, 4)) < 0) {
            user_panic("ide_write failed.(1 sel diskno)");
        }
        
        tmpword = offset + offset_begin; // BUG FIX!
        if ((r = syscall_write_dev(&tmpword, IDE_PADDR + 0x0000, 4)) < 0) {
            user_panic("ide_write failed.(2 sel offset)");
        }

        if ((r = syscall_write_dev(src + offset, IDE_PADDR + 0x4000, BY2SECT) < 0)) {
            user_panic("ide_write failed.(3 write buffer)");
        }

        tmpbyte = 1;
        // writef("*(&reg) = %x\n", (u_int)(*(u_char*)(&reg)));
        if ((r = syscall_write_dev(&tmpbyte, IDE_PADDR + 0x0020, 1)) < 0) {
            user_panic("ide_write failed.(4 set write)");
        }

        if ((r = syscall_read_dev(&tmpbyte, IDE_PADDR + 0x0030, 1)) < 0) {
            user_panic("ide_read failed.(5 fetch status)");
        }
        
        if (tmpbyte == 0) {
            user_panic("ide_read failed.(6 check status)");
        }

        offset += BY2SECT;
	}
#ifdef DEBUG_IDERW
    writef("ide_write: write successfully.\n");
#endif
}

