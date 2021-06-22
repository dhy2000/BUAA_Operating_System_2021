#include "lib.h"
#include "cursor.h"
#include <args.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
/* ^^^^^^ My Extended Super Shell ^^^^^^^^^ */

/* ^^^^^^ Part 1. Constant Text Messages ^^^^^^^^^ */
inline static void printWelcome() {
    writef("\n");
    writef(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    writef("::                                                         ::\n");
    writef("::              \033[31mExtended Shell\033[0m  \033[33mV 2021.6\033[0m                   ::\n");
    writef("::                                                         ::\n");
    writef(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
}

inline static void printhead() {
    cur_color_front(COLOR_FRONT_WHITE);
    writef("mos ");
    cur_color_front(COLOR_FRONT_BLUE);
    writef("$");
    cur_color_restore();
    writef(" ");
}


/* ^^^^^^ Part 2. Text Inputs ^^^^^^^^^ */

/* Input Buffers */
#define INPUT_MAX_LEN 512
// #define SPECIAL_MAX_LEN 16
char input_buf[INPUT_MAX_LEN];
int input_size, cursor_pos; // do not use u_int;
//@ public invariant cursor_pos <= input_size;
//@ public invariant input_buf[input_size] == 0;
//@ public invariant input_size < INPUT_MAX_LEN

inline static void mputc(char ch) {
    if (ch >= 32 && ch <= 126)
        syscall_putchar(ch);
    else
        syscall_putchar('\?');
}


static void reprint() {
    cur_clear_lright();
    int i, cnt = 0;
    for (i = cursor_pos; i < input_size; i++) {
        cnt++; 
        mputc(input_buf[i]);
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

/* ^^^^^^ Part 3. Special Keys ^^^^^^^^^ */


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
    (SpecialKey*)(special_key_data + 18) // DELETE
};

static void key_event_backspace();
static void key_event_delete();
static void key_event_up();
static void key_event_down();
static void key_event_left();
static void key_event_right();

void (*key_event[])(void) = {0, key_event_backspace, key_event_up, key_event_down, key_event_left, key_event_right, key_event_delete};


void key_event_backspace() {
    // writef("@@backspace@@");
    text_cur_move(-1);
    deleteChar();
}

void key_event_delete() {
    // writef("@@delete@@");
    deleteChar();
}

void key_event_up() {
    // TODO: not implemented yet
}

void key_event_down() {
    // TODO: not implemented yet
}

void key_event_left() {
    text_cur_move(-1);
}

void key_event_right() {
    text_cur_move(1);
}

/* Overview: detect whether there is special key sequence at where you just inputted.
 *    If there is a special key sequence, you should delete those characters in the input buffer,
 *    and call certain events of the special key.
 */
static void special_key_detect() {
    int numkey = ARRAY_SIZE(spec_key);
    SpecialKey *sk;
    int i;
    for (i = 0; i < numkey; i++) {
        if (spec_key[i] == 0) continue;
        sk = spec_key[i];
        // input_buf[cursor_pos - sk->len : cursor_pos] equals sk-> chr
        if (cursor_pos < sk->len) continue;
        int ppos = cursor_pos - sk->len;
        int eqflag = 1;
        int j;
        for (j = 0; j < sk->len; j++) {
            if (input_buf[ppos + j] == sk->chr[j]) continue;
            else { eqflag = 0; break; }
        }
        if (eqflag) {
            // Hit!
            text_cur_move(ppos - cursor_pos);
            for (j = 0; j < sk->len; j++) {
                deleteChar();
            }
            if (key_event[i] != 0)
                (key_event[i])(); // run callback
            break;
        }
    }
}


/* ^^^^^^ Part 4. Top Interfaces ^^^^^^^^^ */

static void runcmd(char *s); // declaration

/*
 * Overview: callback method when finished entering a command.
 * Pre-Condition: Finished entering a command and hit LF or CR.
 * Post-Condition: Call command parser to run it, and start a new line.
 */
void inputenter() {
    if (input_size > 0) {
        input_buf[input_size] = 0;
        /* writef("@@@ command:\"%s\"\n", input_buf);
        if (strcmp(input_buf, "exit") == 0) {
            exit();
        } */
        int r;
        if ((r = fork()) < 0) 
            writef("@@ error on fork!\n");
        else {
            if (r == 0) { // child
                runcmd(input_buf);
                exit();
                return;
            } else {
                // father
                // TODO: support background command (not wait)
                wait(r);
            }
        }
    }
    clearbuffer();
    writef("\n");
    printhead();
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
        } else {
            if (ch >= 32 && ch <= 126)
                syscall_putchar(ch);
            else
                syscall_putchar('\?');
            insertChar(ch);
            special_key_detect();
        }
    }
}


/* ^^^^^^ Part 5. Parser for shell ^^^^^^^^^ */

int debug_ = 0;
int interactive = '?';
int echocmds = 0;

//
// get the next token from string s
// set *p1 to the beginning of the token and
// *p2 just past the token.
// return:
//    0 for end-of-string
//    > for >
//    | for |
//    w for a word
//
// eventually (once we parse the space where the nul will go),
// words get nul-terminated.
#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()"

static int
_gettoken(char *s, char **p1, char **p2)
{
    int t;

    if (s == 0) {
        //if (debug_ > 1) writef("GETTOKEN NULL\n");
        return 0;
    }

    //if (debug_ > 1) writef("GETTOKEN: %s\n", s);

    *p1 = 0;
    *p2 = 0;

    while(strchr(WHITESPACE, *s))
        *s++ = 0;
    if(*s == 0) {
    //    if (debug_ > 1) writef("EOL\n");
        return 0;
    }
    if(strchr(SYMBOLS, *s)){
        t = *s;
        *p1 = s;
        *s++ = 0;
        *p2 = s;
//        if (debug_ > 1) writef("TOK %c\n", t);
        return t;
    }
    *p1 = s;
    while(*s && !strchr(WHITESPACE SYMBOLS, *s))
        s++;
    *p2 = s;
    if (debug_ > 1) {
        t = **p2;
        **p2 = 0;
//        writef("WORD: %s\n", *p1);
        **p2 = t;
    }
    return 'w';
}

static int
gettoken(char *s, char **p1)
{
    static int c, nc;
    static char *np1, *np2;

    if (s) {
        nc = _gettoken(s, &np1, &np2);
        return 0;
    }
    c = nc;
    *p1 = np1;
    nc = _gettoken(np2, &np1, &np2);
    return c;
}

#define MAXARGS 16
void
runcmd(char *s)
{
    char *argv[MAXARGS], *t;
    int argc, c, i, r, p[2], fd, rightpipe;
    int fdnum;
    rightpipe = 0;
    gettoken(s, 0);
again:
    argc = 0;
    for(;;){
        c = gettoken(0, &t);
        switch(c){
        case 0:
            goto runit;
        case 'w':
            if(argc == MAXARGS){
                writef("too many arguments\n");
                exit();
            }
            argv[argc++] = t;
            break;
        case '<':
            if(gettoken(0, &t) != 'w'){
                writef("syntax error: < not followed by word\n");
                exit();
            }
            // Your code here -- open t for reading,
            // writef("@@@ shell: will open %s for read\n", t);
            r = open(t, O_RDONLY);
            if (r < 0) { user_panic("sh.c failed to open file"); }
            fd = r;
            dup(fd, 0);
            close(fd);
            
            // dup it onto fd 0, and then close the fd you got.
            // user_panic("< redirection not implemented");
            break;
        case '>':
            // Your code here -- open t for writing,
            
            if(gettoken(0, &t) != 'w'){
                writef("syntax error: < not followed by word\n");
                exit();
            }
            // writef("@@@ shell: will open %s for write\n", t);
            r = open(t, O_WRONLY | O_CREAT);
            if (r < 0) {user_panic("sh.c failed to open file");}
            fd = r;
            dup(fd, 1);
            close(fd);
            // dup it onto fd 1, and then close the fd you got.
            // user_panic("> redirection not implemented");
            break;
        case '|':
            // Your code here.
            //     First, allocate a pipe.
            //    Then fork.
            //    the child runs the right side of the pipe:
            //        dup the read end of the pipe onto 0
            //        close the read end of the pipe
            //        close the write end of the pipe
            //        goto again, to parse the rest of the command line
            //    the parent runs the left side of the pipe:
            //        dup the write end of the pipe onto 1
            //        close the write end of the pipe
            //        close the read end of the pipe
            //        set "rightpipe" to the child envid
            //        goto runit, to execute this piece of the pipeline
            //            and then wait for the right side to finish
            // user_panic("| not implemented");
            
            
            pipe(p);
            if ((rightpipe = fork()) == 0) {
                dup(p[0], 0);
                close(p[0]);
                close(p[1]);
                goto again;
            } else {
                dup(p[1], 1);
                close(p[1]);
                close(p[0]);
                goto runit;
            }

            break;
        }
    }

runit:
    if(argc == 0) {
        if (debug_) writef("EMPTY COMMAND\n");
        return;
    }
    argv[argc] = 0;
    
    // support halt
    if (strcmp(argv[0], "halt") == 0 || strcmp(argv[0], "exit") == 0) {
        syscall_halt();
    }


    if ((r = spawn(argv[0], argv)) < 0)
        writef("error spawn %s: %e\n", argv[0], r);
    close_all();
    if (r >= 0) {
        if (debug_) writef("[%08x] WAIT %s %08x\n", env->env_id, argv[0], r);
        wait(r);
    }
    if (rightpipe) {
        if (debug_) writef("[%08x] WAIT right-pipe %08x\n", env->env_id, rightpipe);
        wait(rightpipe);
    }

    exit();
}

void
usage(void)
{
    writef("usage: extsh [-dix]\n");
    exit();
}

void umain(int argc, char **argv) {

    int r, interactive, echocmds;
    interactive = '?';
    echocmds = 0;
    
    printWelcome();
     
    ARGBEGIN{
    case 'd':
        debug_++;
        break;
    case 'i':
        interactive = 1;
        break;
    case 'x':
        echocmds = 1;
        break;
    default:
        usage();
    }ARGEND
     
    if(argc > 1)
        usage();
    if(interactive == '?')
        interactive = iscons(0);
     
    inputenter();
    mainloop();

}

