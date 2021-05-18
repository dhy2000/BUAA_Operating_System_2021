#include "lib.h"


void umain()
{
	int a = 0;
	int id = 0;
    // writef("\n*** &a=%x, &id=%x ***\n", (u_int)&a, (u_int)&id);
    *((volatile int *)(0x45432100)) = 1;
    *((volatile int *)(0x45532100)) = 1;
	if ((id = fork()) == 0) {
        writef("\n--- child will write shared data ---\n");
		a += 2;

		writef("\tthis is child :a:%d\n", a);
        return;
	}
    
    // */
    
    
    
    writef("\n--- father will write shared data ---\n");
    a++;
    writef("this is father: a:%d\n", a);
	return;
    for (;;) {
		// writef("this is father: a:%d\n", a);
	}
    // */
}
