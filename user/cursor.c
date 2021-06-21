#include "cursor.h"

// #include <stdio.h>

static void myputchar(char ch) {
    // Important!
    syscall_putchar(ch);
}

static void mywritestr(const char *str) {
    const char *p = str;
    for (; *p != 0; p++) {
        myputchar((*p));
    }
}

void cur_clear() {
    // clear contents above
    mywritestr("\033[2J");
    cur_set(1, 1);
}

/*
 * Overview: move cursor by a distance along a direction
 * param: 
 *     dis: distance to move
 *     direction: up/down, or left/right
 */
void cur_move(int dis, int direction) {
    char tmp[32];
    char ctrl = 0;
    int mov = 0; // real distance
    switch (direction)
    {
    case CUR_MOVE_UP:
        if (dis > 0) {
            ctrl = 'A';
            mov = dis;
        } else {
            ctrl = 'B';
            mov = -dis;
        }
        break;
    case CUR_MOVE_DOWN:
        if (dis > 0) {
            ctrl = 'B';
            mov = dis;
        } else {
            ctrl = 'A';
            mov = -dis;
        }
        break;
    case CUR_MOVE_LEFT:
        if (dis > 0) {
            ctrl = 'D';
            mov = dis;
        } else {
            ctrl = 'C';
            mov = -dis;
        }
        break;
    case CUR_MOVE_RIGHT:
        if (dis > 0) {
            ctrl = 'C';
            mov = dis;
        } else {
            ctrl = 'D';
            mov = -dis;
        }
        break;
    default:
        break;
    }
    if (mov > 0) {
        // assert dis > 0;
        swritef(tmp, "\033[%d%c", mov, ctrl);
        mywritestr(tmp);
    }
}

/*
 * Overview: set cursor to a certain position.
 * param:
 *     x: line num, starts at 1
 *     y: col num, starts at 1
 */
void cur_set(int x, int y) {
    if (x < 0 || y < 0) {
        return;
    }
    char tmp[32];
    swritef(tmp, "\033[%d;%dH", x, y);
    mywritestr(tmp);
}

/*
 * Overview: clear the right part of current line after cursor.
 */
void cur_clear_lright() {
    mywritestr("\033[K");
}

/*
 * Overview: make the cursor back to the head of line
 */
void cur_backto_lhead() {
    // 1. write a '\n'
    myputchar(10);
    // 2. move up by a line
    cur_move(1, CUR_MOVE_UP);
    // 3. clear the line
    cur_clear_lright();
}

void cur_color_front(int col) {
    char tmp[32];
    switch (col) {
        case COLOR_FRONT_BLACK      :
        case COLOR_FRONT_RED        :
        case COLOR_FRONT_GREEN      :
        case COLOR_FRONT_YELLOW     :
        case COLOR_FRONT_BLUE       :
        case COLOR_FRONT_PURPLE     :
        case COLOR_FRONT_DEEP_GREEN :
        case COLOR_FRONT_WHITE      :
            swritef(tmp, "\033[%dm", col);
            mywritestr(tmp);
            break;
        default:;
    }
}

void cur_color_back(int col) {
    char tmp[32];
    switch (col) {
        case COLOR_BACK_BLACK      :
        case COLOR_BACK_RED        :
        case COLOR_BACK_GREEN      :
        case COLOR_BACK_YELLOW     :
        case COLOR_BACK_BLUE       :
        case COLOR_BACK_PURPLE     :
        case COLOR_BACK_DEEP_GREEN :
        case COLOR_BACK_WHITE      :
            swritef(tmp, "\033[%dm", col);
            mywritestr(tmp);
            break;
    }
}

void cur_color(int front, int back) {
    cur_color_front(front);
    cur_color_back(back);
}

void cur_color_restore() {
    cur_color(COLOR_FRONT_WHITE, COLOR_BACK_BLACK);
}

