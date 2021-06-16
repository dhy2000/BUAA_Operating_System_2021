#include "lib.h"


char *msg = "Now is the time for all good men to come to the aid of their party.";

void
umain(void)
{
	char buf[100];
	int i, pid, p[2];

	if ((i=pipe(p)) < 0)
		user_panic("pipe: %e", i);

	if ((pid=fork()) < 0)
		user_panic("fork: %e", i);

	if (pid == 0) {
		writef("[%08x] pipereadeof close %d\n", env->env_id, p[1]);
		close(p[1]);
		writef("[%08x] pipereadeof readn %d\n", env->env_id, p[0]);
		i = readn(p[0], buf, sizeof buf-1);
		if (i < 0)
			user_panic("read: %e", i);
		buf[i] = 0;
		if (strcmp(buf, msg) == 0)
			writef("\npipe read closed properly\n");
		else
			writef("\ngot %d bytes: %s\n", i, buf);
		exit();
	} else {
        writef("*** I am the writer!\n");
		writef("[%08x] pipereadeof close %d\n", env->env_id, p[0]);
		writef("*** Writer before close readside: isclosed=%d,%d\n", pipeisclosed((p[0])), pipeisclosed((p[1])));
        close(p[0]);

		writef("*** Writer after close readside: isclosed=%d,%d\n", pipeisclosed((p[0])), pipeisclosed((p[1])));
		writef("[%08x] pipereadeof write %d\n", env->env_id, p[1]);
		writef("*** Writer will write\n");
        if ((i=write(p[1], msg, strlen(msg))) != strlen(msg))
			user_panic("write: %e", i);
        writef("*** Writer write done\n");
		close(p[1]);
	}
	wait(pid);

	if ((i=pipe(p)) < 0)
		user_panic("pipe: %e", i);

	if ((pid=fork()) < 0)
		user_panic("fork: %e", i);

	if (pid == 0) {
		close(p[0]);
		for(;;){
			writef(".");
			if(write(p[1], "x", 1) != 1)
				break;
		}
		writef("\npipe write closed properly\n");
	}
	close(p[0]);
	close(p[1]);
	wait(pid);

	writef("pipe tests passed\n");
}
