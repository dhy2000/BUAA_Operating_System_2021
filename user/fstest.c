#include "lib.h"

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void dispRes(int res, const char *event) {
    if (res == 0) {
        writef("@@succeed: %s\n", event);
    } else if (res < 0) {
        writef("!!failure(%d): %s\n", res, event);
    } else {
        writef("--done(%d): %s\n", res, event);
    }
}

void testCreate() {
    int r;
    r = create("/A", FTYPE_DIR);
    dispRes(r, "1 - create dir A");
    r = create("/B", FTYPE_REG);
    dispRes(r, "2 - create file B");
    r = create("/A/a", FTYPE_REG);
    dispRes(r, "3 - create file /A/a");
    r = create("/B/C", FTYPE_DIR);
    dispRes(r, "4 - create dir /B/C");
    r = create("/A", FTYPE_REG);
    dispRes(r, "5 - re-create file /A");
    r = open("/A/a", O_RDWR);
    dispRes(r, "open /A/a");
    int fd = r;
    r = write(fd, "hello, new file", 15);
    dispRes(r, "write to /A/a");
    r = close(fd);
    dispRes(r, "close /A/a");
    r = open("/A/a", O_RDWR);
    dispRes(r, "reOpen /A/a");

    char buf[512];
    user_bzero(buf, sizeof(buf));
    r = read(fd, buf, 15);
    dispRes(r, "read from file");
    writef("read content: %s\n", buf);

    writef("test create done\n");
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
        writef("fstest: read %d bytes: %s\n", n, buf);
        if (strcmp(buf, msg) != 0) {
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

        testCreate(); 

        writef("@@@ fstest OKAY! @@@\n");
         
        /* while (1) {
            //writef("IDLE!");
        } */
}


