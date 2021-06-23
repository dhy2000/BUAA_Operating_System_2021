#include "lib.h"
#include <args.h>

void testhistory() {
    char buf[128];
    int n = history_getcount();
    writef("history count = %d\n", n);
    char *his1 = "hhh1";
    writef("will store %s\n", his1);
    history_store(his1);
 
    n = history_getcount();
    writef("history count = %d\n", n);
    history_load(0, &buf);
    writef("history[0] = %s\n", buf);
    char *his2 = "hhh2";
    writef("will store %s\n", his2);
    history_store(his2);
    n = history_getcount();
    writef("history count = %d\n", n);
    history_load(0, &buf);
    writef("history[0] = %s\n", buf);
    history_load(1, &buf);
    writef("history[1] = %s\n", buf);
}


void umain(int argc, char **argv) {
    writef("@ history.b : argc = %d\n", argc);

    testhistory();


}
