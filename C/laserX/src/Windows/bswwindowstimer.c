#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif


#if !defined(WINDOWS_H) && defined(WINDOWS)
#include <windows.h>
#define WINDOWS_H
#endif


#if !defined(BSWWINDOWSTIMER_H) && defined(WINDOWS)
#include "bswwindowstimer.h"

uint8_t u8CheckMsTimeout(void)
{
    SYSTEMTIME stTime;

    uint32_t u32TimeMs;
    static uint32_t u32LastMs = 0;

    GetSystemTime(&stTime);
    u32TimeMs = (uint32_t)((stTime.wSecond * 1000) + stTime.wMilliseconds);

    if( u32LastMs == 0 )
    {
        u32LastMs = u32TimeMs;
    }

    uint8_t u8Retval = 0;

    if( u32TimeMs >= (u32LastMs + (int32_t)MS_TICK_TIME) )
    {
        u32LastMs = u32TimeMs;
        u8Retval = 1;
    }

    return u8Retval;
}

#endif
