#ifndef _VAR_H_
#define _VAR_H_

#include <types.h>

int envvar_count();
void envvar_name(int, char *dst);
int envvar_set(const char *name, const char *val, u_char ro);
int envvar_get(const char *name, char *dst);
void envvar_rm(const char *name);
int envvar_isro(const char *name);

#define ENV_VAR_GET 0
#define ENV_VAR_SET 1
#define ENV_VAR_UNSET 2
#define ENV_VAR_COUNT 3
#define ENV_VAR_NAME 4
#define ENV_VAR_SETRO 5
#define ENV_VAR_ISRO 6

#endif
