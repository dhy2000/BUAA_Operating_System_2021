#include "lib.h"

// Global variables are not supported, or fs_init will panic at reading superblock
// ^^^^^^TOO LOW^^^^^^^^^

#define HISTORY_FILENAME "/.history"

static char mhfgetc(int fd) {
    char ch;
    int n;
    n = read(fd, &ch, 1);
    if (n == 1)
        return ch;
    else
        return -1;
}

static void mhfputc(int fd, char c) {
    int n;
    n = write(fd, &c, 1);
}

static void mhfputs(int fd, const char *s) {
    const char *p = s;
    while (*p) {
        mhfputc(fd, (*p));
        p++;
    }
    mhfputc(fd, 10);
}

int history_getcount() {
    int fd = open(HISTORY_FILENAME, O_RDONLY);
    char last = 10, ch;
    int count = 0;
    if (fd < 0) {
        // writef("error history_getcount\n");
        return 0;
    }
    
    while ((ch = mhfgetc(fd)) != -1) {
        if (last == 10 && ch >= 33 && ch <= 126) 
            count++;
        last = ch;
    }
    close(fd);

    return count;
}

void history_store(const char *command) {
    int fd = open(HISTORY_FILENAME, O_WRONLY | O_CREAT);
    if (fd < 0) {
        // writef("error history_store\n");
        return;
    }
    lseek(fd, 0, LSEEK_END);
    mhfputs(fd, command);
    close(fd);
}

void history_load(int index, char *dst) {
    // index starts at zero
    char ch = 10;
    char *p = dst;
    int countlf = 0;
    int fd = open(HISTORY_FILENAME, O_RDONLY);
    lseek(fd, 0, LSEEK_SET);
    if (fd < 0) {
        // writef("error history_load\n");
        return;
    }
    while (countlf < index) {
        ch = mhfgetc(fd);
        if (ch == -1) break;
        if (ch == 10) countlf++;
    }
    
    if (ch != -1 && ch) {
        while (1) {
            ch = mhfgetc(fd);
            if (!ch || ch == -1 || ch == '\n')
                break;
            
            *p = ch;
            p++;
        }
        *p = 0;
    }
    close(fd);
}

void history_clear() {
    remove(HISTORY_FILENAME);
}
