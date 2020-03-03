#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif

#ifndef RTEPRINT_H
#include "rteprint.h"
#endif

#ifndef MENU_H
#define MENU_H
#define LEN_OF_STRING 64

#define MIN_FREQUENCY 0
#define MAX_FREQUENCY 28

#define MIN_TIME 0
#define MAX_TIME 40

#define MIN_POWER 0
#define MAX_POWER 4

#define MIN_WORK 0
#define MAX_WORK 10

#define MIN_REC 0
#define MAX_REC 1

#define TRUE 1
#define FALSE 0

struct menuType {
    void (*const funcKeySelect)(struct menuType* const);
    void (*const funcKeyCancel)(struct menuType* const);
    void (*const funcPrintMenu)(uint8_t);
    const uint8_t u8MinVal;
    uint8_t* const pU8CurVal;
    const uint8_t u8MaxVal;
    const uint8_t u8BackMenuIdx;
    const char* const u8MenuString;
};
typedef struct menuType sMenuType;

struct typeRecord {
    uint8_t valFreq;
    uint8_t valTime;
    uint8_t valPower;
};
typedef struct typeRecord sTypeRecord;

uint8_t u8Buf[LEN_OF_STRING];

extern void initMenu();
extern void output(void (*const out)(void));
extern uint8_t u8Input(void);
extern uint8_t u8Menu(void);
extern uint8_t bCanModify(void);
extern uint8_t u8IsWorking(void);

#endif
