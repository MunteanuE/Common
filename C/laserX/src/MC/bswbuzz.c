#ifndef BSWBUZZ_H
#include "bswbuzz.h"

extern uint8_t initializedPeriph;
uint8_t initializedBuzz = 0;

void initBuzz(void){
    if(initializedPeriph){
        initializedBuzz = 1;
    }
    return;
}

void onBuzz(uint16_t u8Time){
    if(initializedPeriph && initializedBuzz){
    }
    return;
}

void offBuzz(void){
    if(initializedPeriph && initializedBuzz){
    }
    return;
}

#endif
