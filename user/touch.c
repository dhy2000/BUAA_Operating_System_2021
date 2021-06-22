#include "lib.h"
#include <args.h>

void umain(int argc, char **argv)
{
    // writef("@ touch: argc = %d\n", argc);
    if (argc < 2) {
        writef("usage: touch filename");
    }
    int r;
    r = create(argv[1], FTYPE_REG | MKDIR_P);
    if (r < 0) {
        writef("touch %s failed with err code %d\n", argv[1], r);
    }
}
