#ifndef RTEBUZZMAIN_H
#include "rtebuzzmain.h"
void rteBuzzMain(uint16_t u8Param, uint8_t u8Mod){
    switch(u8Mod){
    case ON_BUZZ:
        onBuzz(u8Param);
        break;
    case OFF_BUZZ:
        offBuzz();
        break;
    default:
        break;
    }
    return;
}
#endif
