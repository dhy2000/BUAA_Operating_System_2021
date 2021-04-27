#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.14 ***/
void sched_yield(void)
{
    static int count = 1; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
    
    /*
    count--;
    if (count == 0 || curenv == NULL) {
        if (curenv != NULL) {
            LIST_INSERT_TAIL(&env_sched_list[!point], curenv, env_sched_link);
        }
        if (LIST_EMPTY(&env_sched_list[point])) {
            point = !point;        
        }
        if (!LIST_EMPTY(&env_sched_list[point])) {
            nenv = LIST_FIRST(&env_sched_list[point]);
            LIST_REMOVE(nenv, env_sched_link);
            count = nenv->env_pri;
            env_run(nenv);
        }
    }
    env_run(curenv);
    */
    
    // ^^^^^^EXAM^^^^^^
    unsigned int func1 = 0, func2 = 0, func3 = 0;
    unsigned int pri = 0;
    
#define PRI(env) ( ((env)->env_pri) & 0x7F )
#define FUNC1(env) ( ( ((env)->env_pri) >> 8 ) & 0x7F )
#define FUNC2(env) ( ( ((env)->env_pri) >> 16 ) & 0x7F )
#define FUNC3(env) ( ( ((env)->env_pri) >> 24 ) & 0x7F )


    struct Env *env;
    struct Env *nextEnv = NULL;
    // count--;
    // printf("sched_yield %d\n", count);
    if (curenv != NULL) {
        // extra
        func2 = FUNC2(curenv);
        if (count == func2) {
            curenv->env_status = ENV_NOT_RUNNABLE;
            curenv->trigRunnable = func2 + FUNC3(curenv); 
            // printf("curenv(%d) sleep until %d\n",curenv->env_id, curenv->trigRunnable);
            // will insert into tail of sched list
        }
        // part2
        /*if (FUNC1(curenv) > 0) {
            func1 = FUNC1(curenv);
            // curenv->env_pri -= FUNC1(curenv);
            pri = PRI(curenv);
            if (pri >= func1) {
                curenv->env_pri -= func1;
            } else {
                curenv->env_pri -= pri;
            }
        }*/
        
        LIST_INSERT_TAIL(&env_sched_list[0], curenv, env_sched_link);
    }
    if (LIST_EMPTY(&env_sched_list[0])) {
        panic("^^^^^^WWL HAOZIWEIZHI^^^^^^^^^^");
    }
    u_int max_pri = 0;
    LIST_FOREACH(env, &env_sched_list[0], env_sched_link) {
        // restore zombie process
        if (env->env_status == ENV_NOT_RUNNABLE && env->trigRunnable <= count) {
            // printf("env(%d) wake up at %d\n",env->env_id, count);
            env->trigRunnable = 0;
            env->env_status = ENV_RUNNABLE;
        }
        // schedule
        if (env->env_status == ENV_RUNNABLE && PRI(env) >= max_pri) {
            nextEnv = env;
            max_pri = PRI(env);
        }
    }
    if (nextEnv == NULL) {
        panic("^^^^^^Jiwushiyan Biss^^^^^^^^^");
    }
    LIST_REMOVE(nextEnv, env_sched_link);
    count++;
    env_run(nextEnv);

    panic("^^^^^^sched end^^^^^^");
}
