#include "lib.h"

char mgetc() {
    char ch;
    int n;
    while (1) {
        n = read(0, &ch, 1);
        if (n == 0) continue;
        if (n < 0) {
            return -1;
        }
        return ch;
    }
}


void umain() {
    char ch;
    while ((ch = mgetc()) >= 0) {
        if (ch == 'q') {
            exit();
        }
        writef("@[%02x] ", (int)ch);
    }
    writef("@@@@@@\n");

}
