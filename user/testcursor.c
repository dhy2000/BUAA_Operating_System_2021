#include "lib.h"
#include "cursor.h"
void umain(void) {
    writef("cls!\n");
    cur_clear();
    writef("12345678");
    cur_move(2, CUR_MOVE_LEFT);
    writef("9A"); // 1234569A
    writef("\n^^^^^^TOO LOW^^^^^^^^^"); // invisible
    cur_backto_lhead();
    writef("git commit");

    cur_set(10, 5);
    writef("[10,5]\n11111\n");
    cur_color(COLOR_FRONT_YELLOW, COLOR_BACK_WHITE);
    writef(">>>>>> Begin Test part1 >>>>>>");
    cur_color_restore();
    writef("\n");
    cur_color_front(COLOR_FRONT_PURPLE);
    writef("[PASSED: 3]\n[TOTAL: 5]");
    cur_color_restore();
    writef("\n");
    cur_color_front(COLOR_FRONT_GREEN);
    writef("[ You passed part 1]\n");
    cur_color_front(COLOR_FRONT_YELLOW);
    writef("[ You got 100 (of 100) this time. ]\n");
    cur_color_restore();
    writef("^^^^^^TOO LOW^^^^^^^^^\n");




}
