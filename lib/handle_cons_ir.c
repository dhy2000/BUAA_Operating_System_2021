#include <types.h>

static int rtc_get_time() {
    // Step 1: trigger timer update
    *(u_char*)(0x15000000 + 0xA0000000) = 1;
    // Step 2: read time
    return *(u_int*)(0x15000000 + 0x10 + 0xA0000000);
}

static char cons_getc() {
    return *(char*)(0x10000000 + 0xA0000000);
}

static void cons_halt() {
    *(u_char*)(0x10000000 + 0xA0000000 + 0x10) = 0;
}

void handle_cons_ir(u_int status) {
    static int count = 0;
    static int firsttime = 0;
    static char *buf[128];
    static char len = 0;
    count++;
    *buf[len++] = cons_getc();
    if (count == 1) {
        firsttime = rtc_get_time();
        printf("CP0 STATUS: %x, 1st interrupt: %d\n", status, firsttime);
    }
    else {
        int tim = rtc_get_time();
        printf("interval: %d\n", tim - firsttime);
        if (tim - firsttime >= 5) {
            buf[len] = 0;
            printf("length = %d, string = %s\n", len, buf);
        }
    }


}
