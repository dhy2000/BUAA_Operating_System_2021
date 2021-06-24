#include "lib.h"
#include <var.h>

int user_envvar_count() {
    return syscall_environment_var(ENV_VAR_COUNT, 0, 0, 0);
}

void user_envvar_name(int index, char *dst) {
    syscall_environment_var(ENV_VAR_NAME, dst, 0, index);
}

int user_envvar_set(const char *name, const char *val) {
    return syscall_environment_var(ENV_VAR_SET, name, val, 0);
}

int user_envvar_get(const char *name, char *dst) {
    return syscall_environment_var(ENV_VAR_GET, name, dst, 0);
}

void user_envvar_rm(const char *name) {
    return syscall_environment_var(ENV_VAR_UNSET, name, 0, 0);
}
