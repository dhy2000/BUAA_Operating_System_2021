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
    struct Env *nenv;
    // printf("$$ sched yield: point = %d $$\n", point);
    count--;
    if (count == 0 || curenv == NULL || curenv->env_status != ENV_RUNNABLE) {
        if (curenv != NULL) {
            LIST_INSERT_TAIL(&env_sched_list[!point], curenv, env_sched_link);
        }
        // detect have 1
        u_int hav1 = 0;

        if (LIST_EMPTY(&env_sched_list[point])) {
            // point = !point; 
            hav1 = 0;
        } else {
            nenv = LIST_FIRST(&env_sched_list[point]);
            while (nenv != NULL) {
                if (nenv->env_status == ENV_RUNNABLE) {
                    hav1 = 1;
                    // printf("$$ hav1 $$\n");
                    break;
                }
                nenv = LIST_NEXT(nenv, env_sched_link);
            }
        }
        if (!hav1) {
            point = !point;
            u_int hav2 = 0;
            if (LIST_EMPTY(&env_sched_list[point])) {
                hav2 = 0;
            } else {
                nenv = LIST_FIRST(&env_sched_list[point]);
                while (nenv != NULL) {
                    if (nenv->env_status == ENV_RUNNABLE) {
                        hav2 = 1;
                        // printf("$$ hav2 $$\n");
                        break;
                    }
                    nenv = LIST_NEXT(nenv, env_sched_link);
                }
            }
            if (!hav2) {
                panic("^^^^^^NO RUNNABLE ENV^^^^^^^^^");
            }
        }
        LIST_REMOVE(nenv, env_sched_link);
        count = nenv->env_pri;
        // printf("$$ sched_yield env_run next: %d $$\n", nenv->env_id);
        env_run(nenv);

        /* if (!LIST_EMPTY(&env_sched_list[point])) {
            nenv = LIST_FIRST(&env_sched_list[point]);
            LIST_REMOVE(nenv, env_sched_link);
            count = nenv->env_pri;
            env_run(nenv);
        }*/

    }
    // printf("$$ env_run cur $$\n");
    env_run(curenv);
    panic("^^^^^^sched end^^^^^^");
}
