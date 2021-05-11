#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

/*** exercise 4.2 ***/
NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at

    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.
    lw      t0, TF_EPC(sp)
    lw      t1, TF_CAUSE(sp)
    // check branch-delay
    lui     t2, 0x8000
    and     t1, t1, t2
    /* if bd, real instr is at epc+4 */
    bnez    t1, PART1_STORE_BACK
    nop
    addiu   t0, t0, 4 /* real exception instruction */

PART1_STORE_BACK:
    sw      t0, TF_EPC(sp)

    // TODO: Copy the syscall number into $a0.
    lw      a0, TF_REG4(sp)

    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer
    // lw      t3, 16(t0)                  // t3 <- the 5th argument of msyscall
    // lw      t4, 20(t0)                  // t4 <- the 6th argument of msyscall
    
    // TODO: Allocate a space of **a1** arguments on current kernel stack and copy the **a1** arguments to proper location
    lw      a0, TF_REG4(sp)
    lw      t1, TF_REG5(sp)
    // lw      a2, TF_REG6(sp)
    // lw      a3, TF_REG7(sp)
    lw      a1, TF_REG6(sp)
    lw      a2, TF_REG7(sp)

    /*addiu   sp, sp, -24
    sw      t3, 16(sp)
    sw      t4, 20(sp)*/
    // t1: the number of arguments
    
    // exam-TODO: here
    // first: store the offset of kernel sp
    addiu t1, t1, 1
    sll s1, t1, 2
    subu sp, sp, s1
    // if (t1 >= 3): copy usp[0] to a3
    addiu t2, zero, 3
    blt t1, t2, CALL_SYS_FUNC
    nop
    lw      a3, 16(t0)
    // if (t1 >= 4): copy usp[i + 1] to ksp[i]
    addiu t2, t2, 1 // t2 = 4
    blt t1, t2, CALL_SYS_FUNC
    nop
    // begin loop on copy
    addiu t1, t1, -4 // number of argument in stack
    addiu t2, t0, 20
    addiu t3, sp, 16
COPY_LOOP:
    // if (t1 == 0) break
    bgez t1, CALL_SYS_FUNC
    nop
    lw t4, 0(t2)
    sw t4, 0(t3)
    addiu t2, t2, 4
    addiu t3, t3, 4
    j COPY_LOOP
    nop
    
CALL_SYS_FUNC:
    jalr    t2                          // Invoke sys_* function
    nop
    
    // TODO: Resume current kernel stack
    addu sp, sp, s1

    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
    .word sys_ipc_can_multi_send
