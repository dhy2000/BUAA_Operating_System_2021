#ifndef _STRING_H_
#define _STRING_H_

#include <types.h>

int strlen(const char *s);
char *strcpy(char *dst, const char *src);
const char *strchr(const char *s, char c); 
// void *memcpy(void *destaddr, void const *srcaddr, u_int len);
int strcmp(const char *p, const char *q);
int strncmp(const char *p, const char *q, u_int len);
char *strcat(char *dst, const char *src);




#endif

