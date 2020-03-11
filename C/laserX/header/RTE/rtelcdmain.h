#ifndef BSWLCD_H
#include "bswlcd.h"
#endif

#if !defined(RTELCDMAIN_H) && !defined(LINUX) && !defined(WINDOWS)
#define RTELCDMAIN_H
extern void rteLCDMain(uint8_t);
#endif
