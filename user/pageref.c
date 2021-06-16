#include "lib.h"



static int user_pageref(void *v) {

	u_int pte;

	if (!((* vpd)[PDX(v)]&PTE_V))
		return 0;
	pte = (* vpt)[VPN(v)];
	if (!(pte&PTE_V))
		return 0;
    // writef("--^^-- user_pageref va=%08x, PPN=%d, ref=%d \n", (void*)v, PPN(pte), pages[PPN(pte)].pp_ref);
	return pages[PPN(pte)].pp_ref;

}

static int kern_pageref(void *v) {
    return syscall_get_page_ref(v);
}

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

    // return syscall_get_page_ref(v);

    int usr_ref = user_pageref(v);
    int ker_ref = kern_pageref(v);

    //writef("__--__ usr pageref of va %08x is %d, ker pageref of va %08x is %d\n", v, usr_ref, v, ker_ref);

    return ker_ref;
}



