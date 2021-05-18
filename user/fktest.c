#include "lib.h"


void umain()
{
	int a = 0;
	int id = 0;
    // writef("\n*** &a=%x, &id=%x ***\n", (u_int)&a, (u_int)&id);
	/*if ((id = fork()) == 0) {
		if ((id = fork()) == 0) {
			a += 3;

			for (;;) {
				writef("\t\tthis is child2 :a:%d\n", a);
			}
		}
        writef("\n--- child will write shared data ---\n");
		a += 2;

		for (;;) {
			writef("\tthis is child :a:%d\n", a);
		}
	}
    
    */
    *((volatile int *)(0x45432100)) = 1;
    *((volatile int *)(0x45532100)) = 1;
    
    
    
    writef("\n--- father will write shared data ---\n");
    a++;
    writef("this is father: a:%d\n", a);
	for (;;) {
		// writef("this is father: a:%d\n", a);
	}
}
