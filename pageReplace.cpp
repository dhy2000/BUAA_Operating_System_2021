#include "pageReplace.h"
#include <cstdio>
#include <cassert>
#include <cstddef>

#define MAX_PHY_PAGE 64
#define PAGE_SHIFT 12
#define TOTAL_PAGE (1 << 20)

#define get_Page(x) ((x)>>PAGE_SHIFT)
typedef unsigned int u_int;

struct HNode {
    bool valid;
    u_int offset;
    struct HNode *prev, *next;
};
HNode nodes[TOTAL_PAGE];
extern HNode ntail;
HNode nhead = {.valid = false, .offset = 0, .prev = NULL, .next = &ntail};
HNode ntail = {.valid = false, .offset = 0, .prev = &nhead, .next = NULL};
#define Lhead (nhead.next)
#define Ltail (ntail.prev)

inline void List_Insert_Head(HNode *hnode) {
    // offset is ready
    hnode->valid = 1;
    hnode->next = Lhead;
    hnode->prev = &nhead;
    Lhead = hnode;
}

inline void List_Remove(HNode *hnode) {
    assert(hnode->valid);
    hnode->next->prev = hnode->prev;
    hnode->prev->next = hnode->next;
    hnode->valid = 0;
    // hnode->offset = 0;
}

inline bool Page_Visit(u_int pg) {
    HNode *hnode = &nodes[pg];
    if (hnode->valid) {
        if (hnode->prev != &nhead) {
            List_Remove(hnode);
            List_Insert_Head(hnode);
        }
        return true;
    } else {
        return false;
    }
}

void pageReplace(long * physic_memory, long nwAdd) {
    static int count = 0;
    u_int pg = get_Page(nwAdd);
    // fprintf(stderr, "Addr %d, Page %d\n", nwAdd, pg);
    if (!Page_Visit(pg)) {
        if (count < MAX_PHY_PAGE) {
            physic_memory[count] = pg;
            nodes[pg].offset = count;
            List_Insert_Head(&nodes[pg]);
            count++;
        } else {
            // don't need to replace
            HNode *ndtail = Ltail;
            assert(ndtail != &nhead);
            u_int rep_off = ndtail->offset;
            List_Remove(ndtail);
            nodes[pg].offset = rep_off;
            List_Insert_Head(&nodes[pg]);
            physic_memory[rep_off] = pg;
        }
    }
}

