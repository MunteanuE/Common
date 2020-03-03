#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif


#if !defined(TIME_H) && defined(LINUX)
#include <time.h>
#define TIME_H
#endif


#if !defined(MATH_H) && defined(LINUX)
#include <math.h>
#define MATH_H
#endif


#if !defined(BSWLINUXTIMER_H) && defined(LINUX)
#include "bswlinuxtimer.h"

/* It compares old time with new and will return TRUE if new time bigger than old time plus MS_TICK_TIME. */
uint8_t u8IsMsTimeout(uint8_t f100Ms){

    uint8_t u8Ms = TRUE;
    if(TRUE == f100Ms){
        u8Ms = 100;
    }

    struct timespec stSpec;
    uint32_t u32Ms;
    time_t timeS;

    uint32_t u32TimeMs;
    static uint32_t u32LastMs = 0;

    clock_gettime(CLOCK_REALTIME, &stSpec);

    /* It transforms time in millis.
       Struct timespec has not a milliseconds.
       Need convert nanoseconds in milliseconds.*/
    timeS  = (time_t)stSpec.tv_sec;
    u32Ms = (uint32_t)round(stSpec.tv_nsec / 1.0e6);
    if( u32Ms > 999 ){
        timeS++;
        u32Ms = 0;
    }
    u32TimeMs = (uint32_t)(timeS*1000) + u32Ms;

    /* This first initializes u32Last_ms with time. */
    if( u32LastMs == 0 ){
        u32LastMs = u32TimeMs;
    }

    uint8_t u8Retval = 0;
    if( u32TimeMs >= (u32LastMs + (uint32_t)(MS_TICK_TIME*u8Ms)) ){
        u32LastMs = u32TimeMs;
        u8Retval = 1;
    }
    return u8Retval;
}

#endif
