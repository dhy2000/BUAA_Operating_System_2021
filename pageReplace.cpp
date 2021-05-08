#include "pageReplace.h"
#include <random>
#include <ctime>

#define MAX_PHY_PAGE 64
#define PAGE_SHIFT 12
#define TOTAL_PAGE (1 << 20)

#define get_Page(x) ((x)>>PAGE_SHIFT)
typedef unsigned int u_int;

static u_int cycle = 0;

// ****** Basic Strategy *********
void pgreplace_base(long * physic_memory, long nwAdd) {
    int flag = 0;
    static int clock = 0;
    for (int i = 0; i < MAX_PHY_PAGE; i++) {
        if ((nwAdd >> PAGE_SHIFT) == physic_memory[i]) {
            return ;
        }
    }
    for (int i = 0; i < MAX_PHY_PAGE; i++) {
        if (physic_memory[i] == 0) {
            physic_memory[i] = get_Page(nwAdd);
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        physic_memory[(clock++) % MAX_PHY_PAGE] = get_Page(nwAdd);
    }
}


// ****** Random Strategy *********
u_int getRnd() {
    static std::mt19937 rnd(time(NULL));
    return rnd() % MAX_PHY_PAGE;
}
void pgreplace_random(long * physic_memory, long nwAdd) {
    u_int off = getRnd();
    u_int pg = get_Page(nwAdd);
    while (physic_memory[off] == pg) {
        off = getRnd();
    }
    physic_memory[off] = pg;
}

// ****** Least Recent Used *********
// Linked HashMap
struct HNode {
    bool valid;
    u_int offset;
    struct HNode *prev;
    struct HNode *next;
};
HNode nodes[TOTAL_PAGE];
HNode *l_head = NULL, *l_tail = NULL;
u_int l_size = 0;

inline void List_Insert_Head(HNode *hnode) {
    if (l_head == NULL && l_tail == NULL) {
        l_head = l_tail = hnode;
        hnode->prev = NULL;
        hnode->next = NULL;
    } else {
        hnode->next = l_head;
        l_head->prev = hnode;
        hnode->prev = NULL;
        l_head = hnode;
    }
    hnode->valid = true;
}

/*inline void List_Insert_Tail(HNode *hnode) {
    if (l_head == NULL && l_tail == NULL) {
        l_head = l_tail = hnode;
        hnode->prev = NULL;
        hnode->next = NULL;
    } else {
        l_tail->next = hnode;
        hnode->prev = l_tail;
        l_tail = hnode;
        hnode->next = NULL;
    }
    hnode->valid = true;
}*/

inline void List_Remove_Elem(HNode *hnode) {
    if (hnode->next) {
        hnode->next->prev = hnode->prev;
    } else {
        l_tail = hnode->prev;
    }
    if (hnode->prev) {
        hnode->prev->next = hnode->next;
    } else {
        l_head = hnode->next;
    }
    hnode->next = hnode->prev = NULL;
    hnode->valid = false;
}

inline bool List_Visit(u_int hash) {
    HNode *hnode = &nodes[hash];
    if (!hnode->valid) return false;
    List_Remove_Elem(hnode);
    List_Insert_Head(hnode);
    return true;
}

inline u_int List_Replace(u_int hash) {
    u_int rep = l_tail->offset;
    List_Remove_Elem(l_tail);
    List_Insert_Head(&nodes[hash]);
    return rep;
}

void pgreplace_lru(long *physic_memory, long nwAdd) {
    static u_int count = 0;
    u_int pg = get_Page(nwAdd);
    if (count < MAX_PHY_PAGE) {
        physic_memory[count++] = pg;
        nodes[pg].offset = count - 1;
        List_Insert_Head(&nodes[pg]);
    }
    if (!List_Visit(pg)) {
        u_int rep = List_Replace(pg);
        l_head->offset = rep;
        physic_memory[rep] = pg;
    }
}

// ****** Main *********
static void (*pgreplace)(long *, long) = pgreplace_lru;

void pageReplace(long * physic_memory, long nwAdd) {
    pgreplace(physic_memory, nwAdd);
}

