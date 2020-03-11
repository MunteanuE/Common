#ifndef BSWLCD_H
#include "bswlcd.h"

extern uint8_t initializedPeriph;
uint8_t initializedLCD = 0;

void writeLCD(uint8_t u8SymToPrint){
    if(initializedPeriph && initializedLCD){
            //HAL_WRITE_GPIO(PORTA, u8SymToPrint);
    }
    return;
}

void initLCD(void){
    if(initializedPeriph){
        initializedLCD = 1;
    }
    return;
}

#endif
