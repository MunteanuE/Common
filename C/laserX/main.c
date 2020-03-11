/* You must define in build options LINUX, WINDOWS or AVR */

#ifndef RTEADCINIT_H
#include "rteadcinit.h"
#endif

#ifndef RTEAPBINIT_H
#include "rteapbinit.h"
#endif

#ifndef RTEBUZZINIT_H
#include "rtebuzzinit.h"
#endif

#ifndef RTELCDINIT_H
#include "rtelcdinit.h"
#endif

#ifndef RTENVMINIT_H
#include "rtenvminit.h"
#endif

#ifndef RTEPERIPHINIT_H
#include "rteperiphinit.h"
#endif

#ifndef RTEPRINT_H
#include "rteprint.h"
#endif


#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif

#ifndef TIMER_H
#include "timer.h"
#endif

#ifndef MENU_H
#include "menu.h"
#endif


int32_t main(void){

    rtePeriphInit();
    rteADCInit();
    rteAPBInit();
    rteBuzzInit();
    rteNVMInit();
    rteLCDInit();

    uint8_t f100Ms = FALSE;

    initMenu();

    while(1) {

        f100Ms = u8IsWorking();
        /* It checks keypad with timeout. */
        if( u8MsTimeout(f100Ms) )
        {
            if(0 == u8Menu() ){
                break;
            }
        }
        print(u8Buf, bCanModify());
        f100Ms = FALSE;
    }
    printf("\nDone !!!");
    return 1;
}

//rte are un proc
//numai init si main...
//separ. periph
