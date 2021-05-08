#include "pageReplace.h"
#pragma GCC optimize("Ofast")
#include <cassert>
#include <cstddef>
#include <unordered_map>

// #define DEBUG_MESSAGE
#define NDEBUG

#ifdef DEBUG_MESSAGE
#include <cstdio>
#endif

#define MAX_PHY_PAGE 64
#define PAGE_SHIFT 12

#define get_Page(x) ((x)>>PAGE_SHIFT)
typedef unsigned int u_int;

using std::unordered_map;

struct HNode {
    bool valid;
    u_int p_offset;
    struct HNode *prev;
    struct HNode *next;
};

HNode nodes[MAX_PHY_PAGE];
extern HNode ntail;
HNode nhead = {.valid = 0, .p_offset = 0, .prev = NULL, .next = &ntail};
HNode ntail = {.valid = 0, .p_offset = 0, .prev = &nhead, .next = NULL};
unsigned char ncnt = 0;
#define GET_NEW_NODE (&nodes[ncnt++]) // must ensure ncnt < MAX_PHY_PAGE

unordered_map<u_int, HNode*> pgframe;

inline void List_Insert_Head(HNode *hnode) {
    hnode->next = nhead.next;
    hnode->prev = &nhead;
    nhead.next->prev = hnode;
    nhead.next = hnode;
}

inline void List_Remove(HNode *hnode) {
    hnode->next->prev = hnode->prev;
    hnode->prev->next = hnode->next;
}

inline void List_Visit(HNode *hnode) {
    List_Remove(hnode);
    List_Insert_Head(hnode);
}

inline HNode * List_Replace() {
    HNode *hnode = ntail.prev;
    List_Remove(hnode);
    List_Insert_Head(hnode);
    return hnode;
}

void pageReplace(long * physic_memory, long nwAdd) {
    u_int pg = get_Page(nwAdd);
    if (pgframe.count(pg)) { // exists
        HNode *hnode = pgframe[pg];
        List_Visit(hnode);
#ifdef DEBUG_MESSAGE
        fprintf(stderr, "Page Hit: addr=%d, pg=%d, p_off=%d\n", nwAdd, pg, hnode->p_offset);
#endif
    } else {
        if (ncnt < MAX_PHY_PAGE) {
            // allocate a new page frame
            physic_memory[ncnt] = pg; // fill in
            HNode *hnode = GET_NEW_NODE;
            hnode->p_offset = ncnt - 1;
            hnode->valid = 1;
            List_Insert_Head(hnode);
            pgframe[pg] = hnode;
#ifdef DEBUG_MESSAGE
            fprintf(stderr, "Page Miss: addr=%d, pg=%d, p_off=%d (init)\n", nwAdd, pg, hnode->p_offset);
#endif
        }
        else {
            HNode *hnode = List_Replace();
            u_int p_offset = hnode->p_offset;
            u_int pgold = physic_memory[p_offset];
            assert(pgframe[pgold] == hnode);
            pgframe.erase(pgold);
            pgframe[pg] = hnode;
            physic_memory[p_offset] = pg;
#ifdef DEBUG_MESSAGE
            fprintf(stderr, "Page Miss: addr=%d, pg=%d, p_off=%d (replace)\n", nwAdd, pg, hnode->p_offset);
#endif
        }
    }
}

