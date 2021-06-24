#include "lib.h"
#include "cursor.h"
#include <args.h>

void tree_print(char *name, int depth, int ftype) {
    int i;
    int len;
    for (i = 0; i < depth; i++) {
        writef("--");
    }
    len = strlen(name);
    if (ftype == FTYPE_DIR)
        cur_color_front(COLOR_FRONT_BLUE);
    else {
        if (len > 2 && name[len - 1] == 'b' && name[len - 2] == '.')
            cur_color_front(COLOR_FRONT_GREEN);
        else
            cur_color_front(COLOR_FRONT_WHITE);
    }
    writef("%s", name);
    cur_color_restore();
    writef("\n");
}

void tree_dir1(char *path, int depth) {
    int fd, n;
    struct File f;
    if ((fd = open(path, O_RDONLY)) < 0) {
        write("error on tree: open %s returns %d\n", path, fd);
        exit();
    }
    while ((n = readn(fd, &f, sizeof(f))) == sizeof(f)) {
        if (f.f_name[0]) {
            tree_print(f.f_name, depth, f.f_type);
        }
        if (f.f_type == FTYPE_DIR) {
            char nextpath[MAXPATHLEN];
            strcpy(nextpath, path);
            strcat(nextpath, "/");
            strcat(nextpath, f.f_name);
            tree_dir(nextpath, depth + 1);
        }
    }

}


void tree_dir(char *path, int depth) {
    int r;
    struct Stat st;
    if ((r = stat(path, &st)) < 0) {
        writef("error on tree: stat %s returns %d\n", path, r);
        exit();
    }
    if (st.st_isdir) {
        tree_dir1(path, depth);
    }

}

void umain(int argc, char **argv) {
    // writef("@ tree: argc = %d\n", argc);
    if (argc == 1) {
        tree_dir("/", 0);
    } else {
        tree_dir(argv[1], 0);
    }
}
