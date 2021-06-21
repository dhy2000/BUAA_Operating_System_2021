#ifndef _CURSOR_H_
#define _CURSOR_H_


void cur_clear();
void cur_move(int, int);
void cur_set(int, int);

void cur_clear_lright();
void cur_backto_lhead();

void cur_color_front(int);
void cur_color_back(int);
void cur_color(int, int);
void cur_color_restore();


#define CUR_MOVE_UP         0x01
#define CUR_MOVE_DOWN       0x02
#define CUR_MOVE_LEFT       0x03
#define CUR_MOVE_RIGHT      0x04

#define COLOR_FRONT_BLACK       30
#define COLOR_FRONT_RED         31
#define COLOR_FRONT_GREEN       32
#define COLOR_FRONT_YELLOW      33
#define COLOR_FRONT_BLUE        34
#define COLOR_FRONT_PURPLE      35
#define COLOR_FRONT_DEEP_GREEN  36
#define COLOR_FRONT_WHITE       37

#define COLOR_BACK_BLACK        40
#define COLOR_BACK_RED          41
#define COLOR_BACK_GREEN        42
#define COLOR_BACK_YELLOW       43
#define COLOR_BACK_BLUE         44
#define COLOR_BACK_PURPLE       45
#define COLOR_BACK_DEEP_GREEN   46
#define COLOR_BACK_WHITE        47

#endif

