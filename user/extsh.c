#include "lib.h"
#include "cursor.h"


/* Input Buffers */
#define INPUT_MAX_LEN 512
#define SPECIAL_MAX_LEN 16
char input_buf[INPUT_MAX_LEN];
char special_buf[SPECIAL_MAX_LEN]; // special control character sequence
u_int input_size, special_size;




void inputenter() {
    input_buf[input_size] = 0;
    if (input_size > 0) {
        // TODO: call command parser and runner
        writef("@@@ command:\"%s\"\n", input_buf);
    }
    input_size = 0;
    input_buf[0] = 0;
    writef("\n");
    cur_color_front(COLOR_FRONT_WHITE);
    writef("mos ");
    cur_color_front(COLOR_FRONT_BLUE);
    writef("$");
    cur_color_restore();
    writef(" ");
}

static char mgetc() {
    char ch;
    int n;
    while (1) {
        n = read(0, &ch, 1);
        if (n == 0) continue;
        if (n < 0) {
            return -1;
        }
        return ch;
    }
}
/* 
 * Overview: the main state machine for reading input.
 * As we need to respond special keys(like backspace/delete, left/right, up/down), 
 *     the old `readline` can no longer be in use.
 * When read LF or CR, call `inputenter()` to respond to the command we inputed.
 */ 
void mainloop() {
    char ch;
    int n;
    while (1) {
        ch = mgetc();
        if (ch == '\r' || ch == '\n') {
            inputenter();
        } else if (ch == 'q') {
            return;
        } else if (ch >= 32 && ch <= 126) {
            input_buf[input_size++] = ch; 
        }
    }
}



void umain(int argc, char **argv) {
    writef("\n");
    writef(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    writef("::                                                         ::\n");
    writef("::              \033[31mExtended Shell\033[0m  \033[33mV 2021.6\033[0m                   ::\n");
    writef("::                                                         ::\n");
    writef(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    

    inputenter();
    mainloop();

}
