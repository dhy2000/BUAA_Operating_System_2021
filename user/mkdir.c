#include "lib.h"
#include <args.h>

void mmakedir(const char *path, int recursive) {
    int r;
    int mode = recursive ? (FTYPE_DIR | MKDIR_P) : (FTYPE_DIR);
    r = create(path, mode);
    if (r < 0) {
        writef("mkdir failed with code %d\n", r);
        return;
    }
    
}

void umain(int argc, char **argv) {
    // writef("@ mkdir: argc = %d\n", argc);
    if (argc == 1) {
        writef("usage: mkdir [-p] dirname\n");
        return;
    }
    if (strcmp(argv[1], "-p") == 0) {
        if (argc >= 2) {
            mmakedir(argv[2], 1);
        } else {
            writef("error: need dirname");
        } 
    } else {
        if (argv[1][0] == '-') {
            writef("error: invalid option %s\n", argv[1]);
        } else {
            mmakedir(argv[1], 0);
        }
    }
}
