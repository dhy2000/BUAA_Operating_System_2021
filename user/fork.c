// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>


/* ----------------- help functions ---------------- */

/* Overview:
 * 	Copy `len` bytes from `src` to `dst`.
 *
 * Pre-Condition:
 * 	`src` and `dst` can't be NULL. Also, the `src` area 
 * 	 shouldn't overlap the `dest`, otherwise the behavior of this 
 * 	 function is undefined.
 */
void user_bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	//	writef("~~~~~~~~~~~~~~~~ src:%x dst:%x len:%x\n",(int)src,(int)dst,len);
	max = dst + len;

	// copy machine words while possible
	if (((int)src % 4 == 0) && ((int)dst % 4 == 0)) {
		while (dst + 3 < max) {
			*(int *)dst = *(int *)src;
			dst += 4;
			src += 4;
		}
	}

	// finish remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst = *(char *)src;
		dst += 1;
		src += 1;
	}

	//for(;;);
}

/* Overview:
 * 	Sets the first n bytes of the block of memory 
 * pointed by `v` to zero.
 * 
 * Pre-Condition:
 * 	`v` must be valid.
 *
 * Post-Condition:
 * 	the content of the space(from `v` to `v`+ n) 
 * will be set to zero.
 */
void user_bzero(void *v, u_int n)
{
	char *p;
	int m;

	p = v;
	m = n;

	while (--m >= 0) {
		*p++ = 0;
	}
}
/*--------------------------------------------------------------*/

/* Overview:
 * 	Custom page fault handler - if faulting page is copy-on-write,
 * map in our own private writable copy.
 * 
 * Pre-Condition:
 * 	`va` is the address which leads to a TLBS exception.
 *
 * Post-Condition:
 *  Launch a user_panic if `va` is not a copy-on-write page.
 * Otherwise, this handler should map a private writable copy of 
 * the faulting page at correct address.
 */
/*** exercise 4.13 ***/
static void
pgfault(u_int va)
{
	u_int *tmp;
	writef("^ fork.c:pgfault():\t va:%x\n",va);
    u_int perm = ((Pte*)(*vpt))[VPN(va)] & 0xFFF; 
    if (!(perm & PTE_COW)) {
        user_panic("^^^^^^NOT COW^^^^^^^^^");
    }
    tmp = USTACKTOP;
    perm = (perm | PTE_V | PTE_R) & (~PTE_COW);
    //map the new page at a temporary place
    int r = syscall_mem_alloc(0, tmp, perm);
    if (r < 0) {user_panic("^^^^^^PGFAULT FAILED ALLOC^^^^^^^^^");}
	//copy the content
	va = ROUNDDOWN(va, BY2PG);
    user_bcopy(va, tmp, BY2PG);
    //map the page on the appropriate place
	r = syscall_mem_map(0, tmp, 0, va, perm);
    if (r < 0) {user_panic("^^^^^^PGFAULT FAILED MAP^^^^^^^^^");}
    //unmap the temporary place
    r = syscall_mem_unmap(0, tmp);
    if (r < 0) {user_panic("^^^^^^PGFAULT FAILED UNMAP^^^^^^^^^");}
	// */
}

/* Overview:
 * 	Map our virtual page `pn` (address pn*BY2PG) into the target `envid`
 * at the same virtual address. 
 *
 * Post-Condition:
 *  if the page is writable or copy-on-write, the new mapping must be 
 * created copy on write and then our mapping must be marked 
 * copy on write as well. In another word, both of the new mapping and
 * our mapping should be copy-on-write if the page is writable or 
 * copy-on-write.
 * 
 * Hint:
 * 	PTE_LIBRARY indicates that the page is shared between processes.
 * A page with PTE_LIBRARY may have PTE_R at the same time. You
 * should process it correctly.
 */
/*** exercise 4.10 ***/
static void
duppage(u_int envid, u_int pn)
{
	/*u_int addr;
	u_int perm;
    int r;
    
    addr = pn * BY2PG;
    perm = ( ((Pte*)(*vpt))[pn] ) & 0xFFF;
    writef("^ duppage: pn=%d, perm=%x ^\n", pn, perm);
    if (!(!(perm & PTE_R) || (perm & PTE_LIBRARY) || (perm & PTE_COW))) {
        perm |= PTE_COW;
        r = syscall_mem_map(0, addr, envid, addr, perm);
        if (r < 0) {user_panic("^^^^^^map(child)^^^^^^^^^");}
        r = syscall_mem_map(0, addr, 0, addr, perm);
        if (r < 0) {user_panic("^^^^^^map(father)^^^^^^^^^");}
        writef("^^ duppage done ^^\n");
    } else {
        syscall_mem_map(0, addr, envid, addr, perm);
        if (r < 0) {user_panic("^^^^^^map^^^^^^^^^");}
    }

    // */

    u_int addr;
    u_int perm;
    addr = pn << PGSHIFT;
    perm = ((Pte *)(*vpt))[pn] & 0xfff;


    writef("^ duppage: pn=%d, perm=%x ^\n", pn, perm);
    


    /*writef("^ fake duppage: return 0 ^\n");
    return 0;
    // */
    //writef("%x %x\n",addr, ((Pte *)(*vpt))[pn]);
    
    
    if ((perm & PTE_R) == 0)
    {
        if(syscall_mem_map(0, addr, envid, addr, perm) < 0)
        {
            user_panic("user panic mem map error!1");
        }
    }
    else if (perm & PTE_LIBRARY)
    {
        if(syscall_mem_map(0, addr, envid, addr, perm) < 0)
        {
            user_panic("user panic mem map error!2");
        }
    }
    else if (perm & PTE_COW)
    {
        if(syscall_mem_map(0, addr, envid, addr, perm) < 0)
        {
            user_panic("user panic mem map error!3");
        }
    }
    else
    {   
        if(syscall_mem_map(0, addr, envid, addr, perm | PTE_COW) < 0)
        {
            user_panic("user panic mem map error!4");
        }
        writef("^ done dup child's page ^\n");
        if(syscall_mem_map(0, addr, 0, addr, perm | PTE_COW) < 0)
        {
            user_panic("user panic mem map error!5");
        }
        writef("^ done dup father's page ^\n");
    }
    writef("^^ duppage done ^^\n");
/*    writef("dup dup dup dup dup dup dup dup dup dup\n");
    int i = 0, j = 0, sz = 7;
    for (i = -sz + 1; i < sz; i++) {
        for (j = -sz; j < sz; j++) {
            int absi = i < 0 ? -i : i;
            int absj = j < 0 ? -j : j;
            if (absi + absj == sz - 1) writef("*");
            else writef(" ");
        }
        writef("\n");
    }
    // */


/*    u_int addr;
    u_int perm;

    int flag = 0;

    // writef("4.10 used\n");
    addr = pn << PGSHIFT;
    perm = (*vpt)[pn] & 0xfff;

    if (((perm & PTE_R) != 0) && (perm & PTE_V)) {
        if (perm & PTE_LIBRARY) {
            // static sharing
            perm = PTE_V | PTE_R | PTE_LIBRARY | perm;
        } else {
            // need paste one
            perm = PTE_V | PTE_R | PTE_COW | perm;
        }
        // child map
        if (syscall_mem_map(syscall_getenvid(), addr, envid, addr, perm) < 0) {
            writef("%x\n", addr);
            user_panic("sys_mem_map for son failed.\n");
        }
        // fa map
        if (syscall_mem_map(syscall_getenvid(), addr, syscall_getenvid(), addr,
                            perm) < 0) {
            user_panic("sys_mem_map for father failed.\n");
        }
    } else {
        // read-only or invalid
        if (syscall_mem_map(syscall_getenvid(), addr, envid, addr, perm) < 0) {
            user_panic("sys_mem_map for son failed.1\n");
        }
    }


    writef("^^ duppage done ^^\n");
*/

	//	user_panic("duppage not implemented");
}

/* Overview:
 * 	User-level fork. Create a child and then copy our address space
 * and page fault handler setup to the child.
 *
 * Hint: use vpd, vpt, and duppage.
 * Hint: remember to fix "env" in the child process!
 * Note: `set_pgfault_handler`(user/pgfault.c) is different from 
 *       `syscall_set_pgfault_handler`. 
 */


#define OS_DEBUG_NIGN_FORK
#define OS_DEBUG_NIGN_DUPPAGE


/*** exercise 4.9 4.15***/
extern void __asm_pgfault_handler(void);
int
fork(void)
{
	// Your code here.
	u_int newenvid;
	extern struct Env *envs;
	extern struct Env *env;
	u_int i;


	//The parent installs pgfault using set_pgfault_handler
    set_pgfault_handler(pgfault);
	//alloc a new alloc
    newenvid = syscall_env_alloc();
    if (newenvid < 0) {
        return newenvid; // failure
    }
    writef("!! fork.c: after syscall_env_alloc()\n");
    if (newenvid == 0) { // child
        i = syscall_getenvid();
        env = &envs[ENVX(i)];

    } else { // father
#ifdef OS_DEBUG_NIGN_FORK
        writef("!! fork.c: (father) newenvid = %d\n", newenvid);
#ifdef OS_DEBUG_NIGN_DUPPAGE
        // duppage
        // writef("^ fork: to duppage - i < %d ^\n", VPN(USTACKTOP));
        for (i = 0; i < VPN(USTACKTOP); i++) {
            if ( ( ((Pde*)(*vpd))[(i >> 10)] & PTE_V ) && ( ((Pte*)(*vpt))[(i)] & PTE_V ) ) {
                duppage(newenvid, i);
                writef("!! fork.c: duppage(%d) ok \n", i);
            }
        }
        writef("!! fork.c: duppage done ^\n");
        // */
#endif
        // alloc uxstack
        i = syscall_mem_alloc(newenvid, UXSTACKTOP - BY2PG, PTE_V | PTE_R);
        if (i < 0) {user_panic("^^^^^^err alloc uxstack^^^^^^^^^");}
        writef("!! fork.c: uxstack done ^\n");
        
        
        // set_pgfault_handler
        i = syscall_set_pgfault_handler(newenvid, __asm_pgfault_handler, UXSTACKTOP);
        if (i < 0) {user_panic("^^^^^^err set pgfault handler^^^^^^^^^");}
        writef("!! fork.c: pgfault handl done ^\n");
        
        i = syscall_set_env_status(newenvid, ENV_RUNNABLE);
        if (i < 0) {user_panic("^^^^^^error set child's status^^^^^^^^^");}
        writef("!! fork.c: fork father done ! ^\n");
        // */
#endif
    }
    // */

	return newenvid;
}

// Challenge!
int
sfork(void)
{
	user_panic("sfork not implemented");
	return -E_INVAL;
}
