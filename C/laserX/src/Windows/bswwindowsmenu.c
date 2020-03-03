#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif


#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif


#if !defined(BSWWINDOWSMENU_H) && defined(WINDOWS)
#include "bswwindowsmenu.h"

void printChar(const char u8c)
{
    putchar((int)u8c);
    return;
}

#endif
