#include "lib.h"

int
pageref(void *v)
{
    /*
	u_int pte;

	if (!((* vpd)[PDX(v)]&PTE_V))
		return 0;
	pte = (* vpt)[VPN(v)];
	if (!(pte&PTE_V))
		return 0;
    writef("--^^-- pageref %08x, PPN=%d, ref=%d \n", (void*)v, PPN(pte), pages[PPN(pte)].pp_ref);
	return pages[PPN(pte)].pp_ref;

    */

    return syscall_get_page_ref(v);
}
