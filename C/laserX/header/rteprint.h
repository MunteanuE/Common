#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif

#if defined(WINDOWS) && !defined(BSWWINDOWSMENU_H)
#include "bswwindowsmenu.h"

#elif defined(LINUX) && !defined(BSWLINUXMENU_H)
#include "bswlinuxmenu.h"
#endif

#ifndef RTEPRINT_H
#define RTEPRINT_H
#define TRUE 1
#define FALSE 0
void print(uint8_t*, uint8_t);
extern void printChar(const char);
#endif
