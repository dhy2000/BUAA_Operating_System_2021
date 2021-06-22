#include "lib.h"

int isprime(int n) {
    int i;
    for (i = 2; i <= n - 1; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

void getAllPrime(int mlim) {
    int i = 2;
    int checkpoint = mlim / 10;
    for (; i <= mlim; i++) {
        if (i % checkpoint == 0) {
            writef("@@@ i = %d @@@\n", i);
        }
        if (isprime(i)) ; 
            // writef("@@@ %d is prime @@@\n", i);
    }
}


void umain() {
    writef("@@ A slow program @@\n");
    getAllPrime(100000);
    writef("@@ Done! @@\n");
    return;
}
