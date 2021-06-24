#include <var.h>
#include <libstring.h>

#define MAX_ENVVAR_NUM 16
#define MAX_ENVVAR_LEN 256
int var_count = 0;

char var_names[MAX_ENVVAR_NUM][MAX_ENVVAR_LEN];
char var_values[MAX_ENVVAR_NUM][MAX_ENVVAR_LEN];

static int name2index(const char *name) {
    int i;
    for (i = 0; i < var_count; i++) {
        if (strcmp(var_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

// return the index of the new name
inline static int append_name(const char *name) {
    // assert var_count < MAX_ENVVAR_NUM;
    strcpy(var_names[var_count], name);
    var_count++;
    return var_count - 1;
}

int envvar_count() {
    return var_count;
}

void envvar_name(int index, char *dst) {
    if (index < var_count)
        strcpy(dst, var_names[index]);
}


int envvar_set(const char *name, const char *val) {
    if (var_count >= MAX_ENVVAR_NUM) {
        return -1;
    }
    int n;
    n = name2index(name);
    if (n < 0) {
        n = append_name(name);
    }
    strcpy(var_values[n], val);
    return 0;
}


int envvar_get(const char *name, char *dst) {
    int n;
    n = name2index(name);
    if (n < 0) {
        return -1;
    }
    strcpy(dst, var_values[n]);
    return 0;
}


void envvar_rm(const char *name) {
    int n, i;
    n = name2index(name);
    if (n < 0) return;
    var_count--;
    for (i = n; i < var_count; i++) {
        strcpy(var_names[i], var_names[i + 1]);
        strcpy(var_values[i], var_values[i + 1]);
    }
}
