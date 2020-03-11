#if !defined(RTELCDMAIN_H) && !defined(LINUX) && !defined(WINDOWS)
#include "rtelcdmain.h"
void rteLCDMain(uint8_t u8Param){
    writeLCD(u8Param);
    return;
}
#endif

/* ISO C not forbids empty file */
void foo(void){
    return;
}
