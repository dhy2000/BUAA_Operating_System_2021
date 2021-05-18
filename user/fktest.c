#include "lib.h"

int global_a;

void umain()
{
	int a = 0;
	int id = 0;

	if ((id = fork()) == 0) {
		if ((id = tfork()) == 0) {
			global_a += 3;
            a += 3;
			for (;;) {
				writef("\t\tthis is child2 :ga:%d, a:%d\n", global_a, a);
			}
		}

		global_a += 2;
        a += 2;

		for (;;) {
			writef("\tthis is child :ga:%d, a:%d\n", global_a, a);
		}
	}

	global_a++;
    a++;

	for (;;) {
		writef("this is father: ga:%d, a:%d\n", global_a, a);
	}
}
