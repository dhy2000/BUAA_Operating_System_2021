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
    // writef("@ history.b : argc = %d\n", argc);

    // testhistory();
    int flagclr = 0;
    int hiscount = 0;
    char buf[1024];
    int i;

    if (argc >= 2) {
        if (strcmp(argv[1], "-c") == 0) {
            flagclr = 1;
        }
    }

    if (flagclr) {
        history_clear();
        exit();
    }
    
    hiscount = history_getcount();
    for (i = 0; i < hiscount; i++) {
        history_load(i, buf);
        writef("\033[31m%d\033[0m: \033[34m%s\033[0m\n", i + 1, buf);
    }

}
