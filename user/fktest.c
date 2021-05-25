#include "lib.h"


void umain()
{
	int a = 0;
	int id = 0;

	/*if ((id = fork()) == 0) {
		if ((id = fork()) == 0) {
			a += 3;

			for (;;) {
				writef("\t\tthis is child2 :a:%d\n", a);
			}
		}

		a += 2;

		for (;;) {
			writef("\tthis is child :a:%d\n", a);
		}
	}

	a++;

	for (;;) {
		writef("this is father: a:%d\n", a);
	}*/

    int tim = syscall_get_time();
    writef("time is %d\n", tim);

/*    writef("enter a string:\n");
    char buf[512];
    syscall_read_str(buf, 0);
    writef("You have entered: \"%s\"\n", buf);
  */
    while (1) ;
}
