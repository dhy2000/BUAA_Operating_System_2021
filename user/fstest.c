#include "lib.h"

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void tryCreate(const char *path, int isdir) {
    writef("Trying to create %s:(%d) ...\n", path, isdir);
    int r;
    if ((r = user_create(path, isdir)) < 0) {
        writef("failed to create %s:(%d), err=%d\n", path, isdir, r);
        return;
    }
    writef("successful create %s:(%d)\n", path, isdir);
}

void umain()
{
        int r;
        int fdnum;
        char buf[512];
        int n;

        if ((r = open("/newmotd", O_RDWR)) < 0) {
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
        
        if ((r = user_create("/testfile", 0)) < 0) {
            if (r == -E_DIR_NOT_EXIST) {
                writef("hhhh dir not exist\n");
            }
            user_panic("exam create failed: %d", r);
        }
        writef("file create test: OK\n");
        
        tryCreate("/A", 0);
        tryCreate("/A", 1);
        tryCreate("/A/B", 2);
        tryCreate("/A/B/C", 3);
        
        while (1) {
                //writef("IDLE!");
        }
}


