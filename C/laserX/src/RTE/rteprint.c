#ifndef RTEPRINT_H
#include "rteprint.h"

static void autoFill(uint8_t u8Count)
{
    for(uint8_t u8i = u8Count; u8i < 17; u8i++){
        //rteLCDMain('0'+u8i%10);
        rteLCDMain(' ');
    }
    return;
}

void print(uint8_t* u8Buf, uint8_t bStrCanModify) {

    char cServSymb[] = "\033[A\r";
    const char * ptr = (char*)&cServSymb;
    char c;
    while(*ptr){
        c = *ptr;
        ptr++;
        rteLCDMain(c);
    }

    uint8_t u8Count1 = 0;
    uint8_t flagRep = FALSE;
    if( FALSE == bStrCanModify ) {
        const char * ptrBuff = (char*)u8Buf;
        char charToPrint;
        while(*ptrBuff){

            charToPrint = *ptrBuff;

            if( (charToPrint == '$') && (flagRep == TRUE) ){
                autoFill(u8Count1);
                flagRep = FALSE;
            } else {

                if( (charToPrint == '$') && (flagRep == FALSE) ){
                    u8Count1 = 0;
                    flagRep = TRUE;
                }

                if(flagRep == TRUE){
                    u8Count1++;
                }

            }

            ptrBuff++;

            if(charToPrint != '$' ) {
                rteLCDMain(charToPrint);
            }
        }
        bStrCanModify = 1;
    }

    return;
}

#endif
