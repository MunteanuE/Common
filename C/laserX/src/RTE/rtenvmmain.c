#ifndef RTENVMMAIN_H
#include "rtenvmmain.h"
void rteNVMMain(sTypeRecord* psRec, uint8_t u8Mod){
    switch(u8Mod){
    case WRITE_NVM:
        psRec = readNVM();
        break;
    case READ_NVM:
        writeNVM(psRec);
        break;
    default:
        break;
    }
    return;
}
#endif
