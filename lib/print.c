/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include	<print.h>

/* macros */
#define		IsDigit(x)	( ((x) >= '0') && ((x) <= '9') )
#define		Ctod(x)		( (x) - '0')

/* forward declaration */
extern int PrintChar(char *, char, int, int);
extern int PrintString(char *, char *, int, int);
extern int PrintNum(char *, unsigned long, int, int, int, int, char, int);

extern int PrintS1(char *, void *, int, int, char);
extern int PrintS2(char *, void *, int, int, char);

/* private variable */
static const char theFatalMsg[] = "fatal error in lp_Print!";

/* -*-
 * A low level printf() function.
 */
void
lp_Print(void (*output)(void *, char *, int), 
	 void * arg,
	 char *fmt, 
	 va_list ap)
{

#define 	OUTPUT(arg, s, l)  \
  { if (((l) < 0) || ((l) > LP_MAX_BUF)) { \
       (*output)(arg, (char*)theFatalMsg, sizeof(theFatalMsg)-1); for(;;); \
    } else { \
      (*output)(arg, s, l); \
    } \
  }
    
    char buf[LP_MAX_BUF];

    char c = 0;
    char *s = buf;
    long int num = 0;

	

    int longFlag = 0;
    int negFlag = 0;
    int width = 0;
    int prec = 0;
    int ladjust = 0;
    char padc = 0;

    int length = 0;

    int stypeid = 0;

    void *saddr = arg;

    /*
        Exercise 1.5. Please fill in two parts in this file.
    */

    for(;;) {

        /* Part1: your code here */
    while (*fmt != '\0' && *fmt != '%')
	{ 
	    /* scan for the next '%' */
	    /* flush the string found so far */
        OUTPUT(arg, fmt, 1);
	    /* check "are we hitting the end?" */
        fmt++;
	}
    
    if (*fmt == '\0')
        break;
	
	/* we found a '%' */
	fmt++;
	/* check for long */
    
	/* check for other prefixes */

	/* check format flag */
    
    // %[flags][width][.precsion][length][typeid]specifier

    // [flags]
    if (*fmt == '-') {
        ladjust = 1;
        fmt++;
    } else if (*fmt == '0') {
        padc = '0';
        fmt++;
    } else {
        ladjust = 0;
        padc = ' ';
    }
    // [width]
    width = 0;
    while (IsDigit(*fmt)) {
        width = (width << 3) + (width << 1) + (Ctod(*fmt));
        fmt++;
    }
    // [.precision]
    if (*fmt == '.') {
        fmt++;
        prec = 0;
        while (IsDigit(*fmt)) {
            prec = (prec << 3) + (prec << 1) + (Ctod(*fmt));
            fmt++;
        }
    }
    // [length]
    if (*fmt == 'l') {
        longFlag = 1;
        fmt++;
    }

    // [stypeid]
    if (*fmt == '$') {
        fmt++;
        if (*fmt == '1') {
            stypeid = 1;
        } else {
            stypeid = 2;
        }
        fmt++;
    }
    
	negFlag = 0;
	switch (*fmt) {
	 case 'b':
	    if (longFlag) { 
		num = va_arg(ap, long int); 
	    } else { 
		num = va_arg(ap, int);
	    }
	    length = PrintNum(buf, num, 2, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'd':
	 case 'D':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    
		/*  Part2:
			your code here.
			Refer to other part (case 'b',case 'o' etc.) and func PrintNum to complete this part.
			Think the difference between case 'd' and others. (hint: negFlag).
		*/
        if (num < 0) {
            negFlag = 1;
            num = -num;
        }
	    length = PrintNum(buf, num, 10, negFlag, width, ladjust, padc, 0);
        OUTPUT(arg, buf, length);
		break;

	 case 'o':
	 case 'O':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 8, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'u':
	 case 'U':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 10, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;
	    
	 case 'x':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 16, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'X':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 16, 0, width, ladjust, padc, 1);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'c':
	    c = (char)va_arg(ap, int);
	    length = PrintChar(buf, c, width, ladjust);
	    OUTPUT(arg, buf, length);
	    break;

	 case 's':
	    s = (char*)va_arg(ap, char *);
	    length = PrintString(buf, s, width, ladjust);
	    OUTPUT(arg, buf, length);
	    break;
     
    case 'T':
        saddr = (void*)va_arg(ap, void *);
        if (stypeid == 1)
            // length = PrintChar(buf, '.', width, ladjust);
            length = PrintS1(buf, saddr, width, ladjust, padc);
        else
            // length = PrintChar(buf, '.', width, ladjust);
            length = PrintS2(buf, saddr, width, ladjust, padc);
        OUTPUT(arg, buf, length);
        break;


	 case '\0':
	    fmt --;
	    break;

	 default:
	    /* output this char as it is */
	    OUTPUT(arg, fmt, 1);
	}	/* switch (*fmt) */

	fmt ++;
    }		/* for(;;) */

    /* special termination call */
    OUTPUT(arg, "\0", 1);
}


/* --------------- local help functions --------------------- */
struct s1 {
    int a;
    char b;
    char c;
    int d;
};

    
int
PrintS1(char * buf, void * addr, int width, int ladjust, char padc)
{
    int length = 0;
    char *head = buf;
    struct s1 *ps1 = (struct s1*) addr;
    // '{'
    *(head++) = '{';
    // int a
    long int na = ps1->a;
    if (na < 0)
        length = PrintNum(head, -na, 10, 1, width, ladjust, padc, 0);
    else
        length = PrintNum(head, na, 10, 0, width, ladjust, padc, 0);
    head += length;
    // ','
    *(head++) = ',';
    // char b
    length = PrintChar(head, ps1->b, width, ladjust);
    head += length;
    // ','
    *(head++) = ',';
    // char c
    length = PrintChar(head, ps1->c, width, ladjust);
    head += length;
    // ','
    *(head++) = ',';
    // int d
    long int nd = ps1->d;
    if (nd < 0)
        length = PrintNum(head, -nd, 10, 1, width, ladjust, padc, 0);
    else
        length = PrintNum(head, nd, 10, 0, width, ladjust, padc, 0);
    head += length;
    // '}'
    *(head++) = '}';
    return head - buf;
}

struct s2 {
    int size;
    int c[];
};

int
PrintS2(char * buf, void * addr, int width, int ladjust, char padc)
{
    int length = 0;
    char *head = buf;
    struct s2 *ps2 = (struct s2*) addr;
    int i = 0;
    // '{'
    *(head++) = '{';
    // int size
    long int s2size = ps2->size;
    if (s2size < 0)
        length = PrintNum(head, -s2size, 10, 1, width, ladjust, padc, 0);
    else
        length = PrintNum(head, s2size, 10, 0, width, ladjust, padc, 0);
    head += length;
    for (i = 0; i < s2size; i++) {
        // ','
        *(head++) = ',';
        // num
        long int ai = ps2->c[i];
        if (ai < 0)
            length = PrintNum(head, -ai, 10, 1, width, ladjust, padc, 0);
        else
            length = PrintNum(head, ai, 10, 0, width, ladjust, padc, 0);
        head += length;
    }
    // '}'
    *(head++) = '}';

    return head - buf;
}
    
    
int
PrintChar(char * buf, char c, int length, int ladjust)
{
    int i;
    
    if (length < 1) length = 1;
    if (ladjust) {
	*buf = c;
	for (i=1; i< length; i++) buf[i] = ' ';
    } else {
	for (i=0; i< length-1; i++) buf[i] = ' ';
	buf[length - 1] = c;
    }
    return length;
}

int
PrintString(char * buf, char* s, int length, int ladjust)
{
    int i;
    int len=0;
    char* s1 = s;
    while (*s1++) len++;
    if (length < len) length = len;

    if (ladjust) {
	for (i=0; i< len; i++) buf[i] = s[i];
	for (i=len; i< length; i++) buf[i] = ' ';
    } else {
	for (i=0; i< length-len; i++) buf[i] = ' ';
	for (i=length-len; i < length; i++) buf[i] = s[i-length+len];
    }
    return length;
}

int
PrintNum(char * buf, unsigned long u, int base, int negFlag, 
	 int length, int ladjust, char padc, int upcase)
{
    /* algorithm :
     *  1. prints the number from left to right in reverse form.
     *  2. fill the remaining spaces with padc if length is longer than
     *     the actual length
     *     TRICKY : if left adjusted, no "0" padding.
     *		    if negtive, insert  "0" padding between "0" and number.
     *  3. if (!ladjust) we reverse the whole string including paddings
     *  4. otherwise we only reverse the actual string representing the num.
     */

    int actualLength =0;
    char *p = buf;
    int i;

    do {
	int tmp = u %base;
	if (tmp <= 9) {
	    *p++ = '0' + tmp;
	} else if (upcase) {
	    *p++ = 'A' + tmp - 10;
	} else {
	    *p++ = 'a' + tmp - 10;
	}
	u /= base;
    } while (u != 0);

    if (negFlag) {
	*p++ = '-';
    }

    /* figure out actual length and adjust the maximum length */
    actualLength = p - buf;
    if (length < actualLength) length = actualLength;

    /* add padding */
    if (ladjust) {
	padc = ' ';
    }
    if (negFlag && !ladjust && (padc == '0')) {
	for (i = actualLength-1; i< length-1; i++) buf[i] = padc;
	buf[length -1] = '-';
    } else {
	for (i = actualLength; i< length; i++) buf[i] = padc;
    }
	    

    /* prepare to reverse the string */
    {
	int begin = 0;
	int end;
	if (ladjust) {
	    end = actualLength - 1;
	} else {
	    end = length -1;
	}

	while (end > begin) {
	    char tmp = buf[begin];
	    buf[begin] = buf[end];
	    buf[end] = tmp;
	    begin ++;
	    end --;
	}
    }

    /* adjust the string pointer */
    return length;
}
