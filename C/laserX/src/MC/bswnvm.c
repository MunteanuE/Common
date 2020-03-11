#ifndef BSWNVM_H
#include "bswnvm.h"

extern uint8_t initializedPeriph;
uint8_t initializedEEPROM = 0;

extern void blockPage(void);
extern void unblockPage(void);
extern void eraseSectEEPROM(void);
extern void writeMemory(void);

sTypeRecord* readNVM(void){
    if(initializedPeriph && initializedEEPROM){
    }
    return 0;
}

void writeNVM(sTypeRecord* psRec){
    if(initializedPeriph && initializedEEPROM){
        //unblockPage();
        //eraseSectEEPROM();
        //writeMemory();
        //blockPage();
    }
    return;
}

void initNVM(void){
    if(initializedPeriph){
        //readNVM();
        checkAllRecs();
        initializedEEPROM = 1;
    }
    return;
}

#endif
