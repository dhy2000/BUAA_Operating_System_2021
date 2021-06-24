#include "lib.h"

char var_value[256];

void
umain(int argc, char **argv)
{
    int i, nflag;
    int r;

    nflag = 0;
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        nflag = 1;
        argc--;
        argv++;
    }   
    for (i = 1; i < argc; i++) {
        if (i > 1)
            write(1, " ", 1);
        if (argv[i][0] == '$') {
            r = user_envvar_get((char*)(argv[i] + 1), var_value);
            if (r == 0) 
                write(1, var_value, strlen(var_value));
        }
        else
            write(1, argv[i], strlen(argv[i]));
    }   
    if (!nflag)
        write(1, "\n", 1); 
}
