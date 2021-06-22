#include "lib.h"
#include "cursor.h"


/* Input Buffers */
#define INPUT_MAX_LEN 512
// #define SPECIAL_MAX_LEN 16
char input_buf[INPUT_MAX_LEN];
int input_size, cursor_pos; // do not use u_int;
//@ public invariant cursor_pos <= input_size;
//@ public invariant input_buf[input_size] == 0;
//@ public invariant input_size < INPUT_MAX_LEN

static void reprint() {
    cur_clear_lright();
    int i, cnt = 0;
    for (i = cursor_pos; i < input_size; i++) {
        cnt++;
        syscall_putchar(input_buf[i]);
    }
    cur_move(cnt, CUR_MOVE_LEFT);// */

}

inline static void clearbuffer() {
    input_size = 0;
    input_buf[0] = 0;
    cursor_pos = 0;
}

static void insertChar(char ch) {
    int i;
    if (input_size >= INPUT_MAX_LEN - 1) return;
    
    for (i = input_size; i >= cursor_pos; i--) {
        input_buf[i + 1] = input_buf[i];
    }
    input_buf[cursor_pos] = ch;
    cursor_pos++;
    input_size++;
    input_buf[input_size] = 0;

    reprint();
}

static void deleteChar() {
    // delete the char at cursor_pos (behavior like `del`)
    // if backspace, please move the cursor left by 1 step and call this function.
    if (cursor_pos == input_size) return;
    input_size--;
    int i;
    for (i = cursor_pos; i < input_size; i++) {
        input_buf[i] = input_buf[i + 1];
    }
    input_buf[input_size] = 0;

    reprint();
}


static void text_cur_move(int steps) {
    // <0: left, >0: right
    if (steps < 0) {
        steps = -steps;
        if (cursor_pos - steps < 0) 
            steps = cursor_pos;
        cursor_pos -= steps;
        cur_move(steps, CUR_MOVE_LEFT);
    } else {
        if (cursor_pos + steps > input_size) {
            steps = input_size - cursor_pos;
        }
        cursor_pos += steps;
        cur_move(steps, CUR_MOVE_RIGHT);
    }

}

/* Special Key Sequence */
// #define SPECIAL_KEY_ESCAPE 0
#define SPECIAL_KEY_BACKSPACE 1
#define SPECIAL_KEY_UP      2
#define SPECIAL_KEY_DOWN    3
#define SPECIAL_KEY_LEFT    4
#define SPECIAL_KEY_RIGHT   5
#define SPECIAL_KEY_DELETE  6
typedef struct {
    u_char len;
    char chr[0];
} SpecialKey;
char special_key_data[] = {1, 0x7f, 3, 0x1b, 0x5b, 0x41, 3, 0x1b, 0x5b, 0x42, 3, 0x1b, 0x5b, 0x44, 3, 0x1b, 0x5b, 0x43, 4, 0x1b, 0x5b, 0x33, 0x7e};
SpecialKey *spec_key[] = {
    0, 
    (SpecialKey*)(special_key_data + 0), // BACKSPACE
    (SpecialKey*)(special_key_data + 2), // UP
    (SpecialKey*)(special_key_data + 6), // DOWN
    (SpecialKey*)(special_key_data + 10), // LEFT
    (SpecialKey*)(special_key_data + 14), // RIGHT
    (SpecialKey*)(special_key_data + 17) // DELETE
};


void inputenter() {
    if (input_size > 0) {
        input_buf[input_size] = 0;
        // TODO: call command parser and runner
        writef("@@@ command:\"%s\"\n", input_buf);
    }
    clearbuffer();
    writef("\n");
    cur_color_front(COLOR_FRONT_WHITE);
    writef("mos ");
    cur_color_front(COLOR_FRONT_BLUE);
    writef("$");
    cur_color_restore();
    writef(" ");
}

/* 
 * Overview: the main state machine for reading input.
 * As we need to respond special keys(like backspace/delete, left/right, up/down), 
 *     the old `readline` can no longer be in use.
 * When read LF or CR, call `inputenter()` to respond to the command we inputed.
 */ 
void mainloop() {
    char ch;
    while (1) {
        ch = syscall_cgetc();
        if (ch == '\r' || ch == '\n') {
            syscall_putchar('\n');
            inputenter();
        } 
        else if (ch == 'q') {
            return;
        } 
        else if (ch == 'b') {
            // BACKSPACE
            text_cur_move(-1);
            deleteChar();
        } 
        else if (ch == 'a') {
            text_cur_move(-1);
        }
        else if (ch == 'd') {
            text_cur_move(1);
        }
        else if (ch == 'x') {
            deleteChar();
        }
        else if (ch >= 32 && ch <= 126) {
            syscall_putchar(ch);
            insertChar(ch);
            // reprint();
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
