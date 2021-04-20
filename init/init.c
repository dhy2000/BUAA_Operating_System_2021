#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>


void fuck() {
    printf("Fuck List\n");
// physical_memory_manage_check();
    
    printf("Fuck It\n");
    struct Env *e1;
    env_alloc(&e1, 0);
    printf("e1->env_id = %d\n", e1->env_id);
    lab3_output(e1->env_id);
    struct Env *e2;
    printf("hhh1\n");
    u_int e2_id = fork(e1);
    printf("hhh2\n");
    envid2env(e2_id, &e2, 0);
    printf("hhh\n");
    lab3_output(e1->env_id);
    lab3_output(e2->env_id);
    printf("------\n");
    u_int e3_id = fork(e1);
    u_int e4_id = fork(e1);
    u_int e5_id = fork(e1);
    lab3_output(e1->env_id);
    lab3_output(e2->env_id);
    lab3_output(e3_id);
    lab3_output(e4_id);
    lab3_output(e5_id);
    struct Env *e4;
    envid2env(e4_id, &e4, 0);
    u_int e6_id = fork(e4);
    lab3_output(e6_id);
    lab3_output(e4_id);
    printf("sum: e1=%d, e2=%d, e3=%d,e4=%d,e5=%d,e6=%d\n", 
            lab3_get_sum(e1->env_id),
            lab3_get_sum(e2->env_id),
            lab3_get_sum(e3_id),
            lab3_get_sum(e4_id),
            lab3_get_sum(e5_id),
            lab3_get_sum(e6_id) 
    );
    printf("^^^EXTRA^^^\n");
    lab3_kill(e4_id);

    printf("sum: e1=%d, e2=%d, e3=%d,e5=%d,e6=%d\n", 
            lab3_get_sum(e1->env_id),
            lab3_get_sum(e2->env_id),
            lab3_get_sum(e3_id),
            lab3_get_sum(e5_id),
            lab3_get_sum(e6_id) 
    );
}


void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();
	
	mips_vm_init();
	page_init();
	
	env_init();
	env_check();
    
    fuck();

	/*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
	 * code_b.c*/
    /*** exercise 3.9 ***/
	/*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
	 * interesting, have fun please*/
    // ENV_CREATE_PRIORITY(user_A, 2);
    // ENV_CREATE_PRIORITY(user_B, 1);

	
	trap_init();
	kclock_init();
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}

void bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	max = dst + len;
	// copy machine words while possible
	while (dst + 3 < max)
	{
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len)
{
	void *max;

	max = b + len;

	//printf("init.c:\tzero from %x to %x\n",(int)b,(int)max);
	
	// zero machine words while possible

	while (b + 3 < max)
	{
		*(int *)b = 0;
		b+=4;
	}
	
	// finish remaining 0-3 bytes
	while (b < max)
	{
		*(char *)b++ = 0;
	}		
	
}
