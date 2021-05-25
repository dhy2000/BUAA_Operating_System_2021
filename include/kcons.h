
#ifndef _KCONS_H_
#define _KCONS_H_
#include <types.h>

#define	IO_CONS		0xb0000000		/* CONS port */
#ifndef __ASSEMBLER__
void kcons_init(void);
void handle_cons_ir(u_int status);
#endif /* !__ASSEMBLER__ */
#endif
