#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif


#ifdef AVR
#define u8IsMsTimeout() 1

#elif defined(WINDOWS) && !defined(BSWWINDOWSTIMER_H)
#include "bswwindowstimer.h"

#elif defined(LINUX) && !defined(BSWLINUXTIMER_H)
#include "bswlinuxtimer.h"
#endif


#ifndef TIMER_H
#define TIMER_H

extern uint8_t u8MsTimeout(uint8_t f100Ms);

#endif
