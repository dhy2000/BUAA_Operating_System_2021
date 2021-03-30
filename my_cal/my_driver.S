#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


.text

LEAF(_my_getchar)
    .set mips2
    .set reorder
    /* $v0: return */

    lui t0, 0xb000 /* start address */
    loop:
        lb v0, 0(t0)
        beq v0, zero, loop
        nop
    /* display back */
    sb v0, 0(t0)
    
    /* check '\r' */
    
    addiu t1, zero, 13
    bne v0, t1, EX
    nop

    /* an extra '\n' */
    addiu t1, zero, 10
    sb t1, 0(t0)
    
    EX:

    jr ra
    nop
END(_my_getchar)

LEAF(_my_putchar)
    .set mips2
    /* $a0 ch */
    lui t0, 0xb000 /* start address */
    
    andi t1, a0, 0xff /* last byte */
    
    /* addiu t1, zero, 0x41 */

    sb t1, 0(t0) /* write */

    jr ra
    nop
END(_my_putchar)

LEAF(_my_exit)
    .set mips2
    lui t0, 0xb000 /* start address */

    addiu t1, zero, 0x233

    sb t1, 0x10(t0)

    jr ra
    nop
END(_my_exit)

