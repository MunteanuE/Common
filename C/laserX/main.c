/* You must define in build options LINUX, WINDOWS or AVR */

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
