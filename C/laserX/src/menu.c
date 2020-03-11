#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif

#ifndef STRING_H
#include <string.h>
#define STRING_H
#endif


#ifndef MENU_H
#include "menu.h"

#define READ_MENU 0
#define PREPARE_TO_CHANGE_MENU 1
#define CHANGE_UP 11
#define CHANGE_DOWN 21
#define SAVE_VALUE 2

#define LINE_1 1
#define LINE_2 2

enum{
    enMenuMain = 0,
    enMenuFrequency,
    enMenuTime,
    enMenuPower,
    enMenuWork,
    enMenuMax,
    enMenuWorking,
};

static uint8_t u8LastKeyValue = 0;
static uint8_t bStrCanModify = 1;
static uint8_t u8TempCur = enMenuFrequency;

static uint16_t u16FreqCur2Phys(uint8_t);
static uint16_t u16TimeCur2Phys(uint8_t);
static uint16_t u16PowerCur2Phys(uint8_t);
static void UpCur(sMenuType* const);
static void DownCur(sMenuType* const);
static uint8_t u8GetPressButt(void);
static void printFreq(uint8_t);
static void writeFreq(char* c, uint8_t);
static void printTime(uint8_t);
static void writeTime(char* c, uint8_t);
static void printPower(uint8_t);
static void writePower(char* c, uint8_t);
static void printMain(uint8_t);
static void printWork(uint8_t);
static void writeWork(char* c, uint8_t);
static void printWorking(uint8_t);
static char* pcOptionMenu(uint8_t);

static void hMainKeySelect(sMenuType* const);
static void hMainKeyCancel(sMenuType* const);

static void KeyUp(sMenuType* const);
static void KeyDown(sMenuType* const);

static void hKeySelectFreq(sMenuType* const);
static void hKeySelectTime(sMenuType* const);
static void hKeySelectPower(sMenuType* const);
static void hKeySelectWork(sMenuType* const);
static void hKeySelectWorking(sMenuType* const);
static void hKeyCancelFreq(sMenuType* const);
static void hKeyCancelTime(sMenuType* const);
static void hKeyCancelPower(sMenuType* const);
static void hKeyCancelWork(sMenuType* const);
static void hKeyCancelWorking(sMenuType* const);

static void offerRec(void);
static void readRec(uint8_t);


const char strMenuFreq[] = "$Menu Frequency$\n$%s$";
const char strMenuTime[] = "$Menu Time$\n$%s$";
const char strMenuPower[] = "$Menu Power$\n$%s$";
const char strMenuWork[] = "%s";
const char strMenuMain[] = "$Select Menu$\n$%s$";
const char strStart[] = "\033[?25l";

char pOptFreq[] = "Frequency";
char pOptTime[] = "Time";
char pOptPower[] = "Power";
char pOptWork[] = "Work";

static uint8_t u8ActiveMenu = 0;

uint8_t u8CurFreq = 0;
uint8_t u8CurTime = 0;
uint8_t u8CurPower = 0;
uint8_t u8CurWork = 0;
uint8_t u8CurWorking = 0;
uint8_t u8CurMain = enMenuFrequency;
uint16_t u16TimeRemaining = 0;

sMenuType menuFrequency = {hKeySelectFreq,    hKeyCancelFreq,    printFreq,    MIN_FREQUENCY,   &u8CurFreq,    MAX_FREQUENCY, enMenuMain, strMenuFreq};
sMenuType menuTime = {     hKeySelectTime,    hKeyCancelTime,    printTime,    MIN_TIME,        &u8CurTime,    MAX_TIME,      enMenuMain, strMenuTime};
sMenuType menuPower = {    hKeySelectPower,   hKeyCancelPower,   printPower,   MIN_POWER,       &u8CurPower,   MAX_POWER,     enMenuMain, strMenuPower};
sMenuType menuWork = {     hKeySelectWork,    hKeyCancelWork,    printWork,    MIN_WORK,        &u8CurWork,    MAX_WORK,      enMenuMain, strMenuWork};
sMenuType mainMenu = {     hMainKeySelect,    hMainKeyCancel,    printMain,    enMenuFrequency, &u8CurMain,    enMenuWork,    0,          strMenuMain};
sMenuType menuWorking = {  hKeySelectWorking, hKeyCancelWorking, printWorking, 0,               &u8CurWorking, 0,             enMenuWork, 0};

sMenuType* asMenuInstances[enMenuMax+2] = {&mainMenu, &menuFrequency, &menuTime, &menuPower, &menuWork, 0, &menuWorking};
const sTypeRecord zeroRecord = {0xFF, 0xFF, 0xFF};
const sTypeRecord defaultRecord = {0x0, 0x0, 0x0};
sTypeRecord asRecordArray[MAX_WORK] = {zeroRecord, zeroRecord, zeroRecord, zeroRecord, zeroRecord,
                                       zeroRecord, zeroRecord, zeroRecord, zeroRecord, zeroRecord};

uint8_t bCanModify(void){
    return bStrCanModify;
}

/*It creates a linked list with structs stMenuType */
void initMenu() {
    bStrCanModify = FALSE;
    sprintf((char*)u8Buf, "%s", (char*)strStart);
    print(u8Buf, bStrCanModify);
    mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal));
    print(u8Buf, bStrCanModify);
    return;
}

uint16_t u16FreqCur2Phys(uint8_t u8Cur){

    uint16_t u16Temp = 0;

    u16Temp = (u8Cur-18)*1000;

    if( u8Cur < 19 ){
        u16Temp = (u8Cur-9)*100;
    }

    if( u8Cur < 10 ){
        u16Temp = u8Cur * 10;
    }
    return u16Temp;
}

static uint16_t u16TimeCur2Phys(uint8_t u8Cur){
    uint16_t u16Temp = (u8Cur-10)*60;

    if( u8Cur < 11 ){
        u16Temp = (u8Cur+1)*5;
    }

    return u16Temp;
}

static uint16_t u16PowerCur2Phys(uint8_t u8Cur){
    uint8_t u8Temp = (u8Cur+1)*5;

    if( 3 == u8Cur ){
        u8Temp = 25;
    }

    if( 4 == u8Cur ){
        u8Temp = 50;
    }

    return u8Temp;
}

static char* pcOptionMenu(uint8_t u8Cur){
    char* c;
    switch(u8Cur){
    case enMenuFrequency:
        c = pOptFreq;
        break;
    case enMenuTime:
        c = pOptTime;
        break;
    case enMenuPower:
        c = pOptPower;
        break;
    case enMenuWork:
        c = pOptWork;
        break;
    default:
        c = 0;
        break;
    }
    return c;
}

static void UpCur(sMenuType* const struc) {
     if( u8TempCur < (*struc).u8MaxVal ){
        u8TempCur++;
     }
     return;
}

static void DownCur(sMenuType* const struc) {
     if( u8TempCur > (*struc).u8MinVal ){
        u8TempCur--;
     }
    return;
}

/* It is handler for long press button. */
uint8_t u8Input(void) {
    uint8_t u8Result = 0;
    uint8_t u8PressButt = u8GetPressButt();

    if( u8PressButt != u8LastKeyValue ) {
        u8LastKeyValue = u8PressButt;
        u8Result = u8PressButt;
    }

    else if( u8LastKeyValue != 0 ) {
        u8Result = u8PressButt;
    }

    return u8Result;
}

/* It accepts value from buttons. */
static uint8_t u8GetPressButt(void) {
    uint8_t u8Result = KEY_ESCAPE;
    u8Result = u8Getch();
    return u8Result;
}
/* It writes physical value of frequency */
static void printFreq(uint8_t u8Val) {

    bStrCanModify = FALSE;
    char c[16];
    writeFreq(c, u8Val);
    sprintf((char*)u8Buf, menuFrequency.u8MenuString, c);
    return;
}

static void writeFreq(char* c, uint8_t u8Val){

    uint16_t u16Temp = u16FreqCur2Phys(u8Val);
    if( u16Temp >= 1000 ){
        u16Temp /= 1000;
        sprintf((char*)c, "%3d kHz", u16Temp);
    } else {
        sprintf((char*)c, "%3d Hz", u16Temp);
    }
    *(c+7) = 0;
    return;
}

/* It writes physical value of time */
static void printTime(uint8_t u8Val) {


    bStrCanModify = FALSE;
    char c[16];
    writeTime(c, u8Val);

    sprintf((char*)u8Buf, menuTime.u8MenuString, c);
    return;
}

static void writeTime(char* c, uint8_t u8Val){

    uint16_t u16Temp = u16TimeCur2Phys(u8Val);
    sprintf((char*)c, "%02d:%02d", u16Temp/60, u16Temp%60);
    *(c+7) = 0;
    return;
}

/* It print physical value from raw value of power */
static void printPower(uint8_t u8Val) {

    bStrCanModify = FALSE;
    char c[16];
    writePower(c, u8Val);

    sprintf((char*)u8Buf, menuPower.u8MenuString, c);

    return;
}

static void writePower(char* c, uint8_t u8Val){

    sprintf((char*)c, "%3d mW", u16PowerCur2Phys(u8Val));
    *(c+7) = 0;
    return;
}

static void printMain(uint8_t u8Val) {

    bStrCanModify = FALSE;

    sprintf((char*)u8Buf, mainMenu.u8MenuString, pcOptionMenu(u8Val));

    return;
}

static void printWork(uint8_t u8Val){

    char c[40];
    char pStrWork[8];
    writeWork(pStrWork, u8Val);

    bStrCanModify = FALSE;
    char pStrFreq[8];
    char pStrTime[8];
    char pStrPower[8];

    if( 0 == u8Val ){
        writeFreq(pStrFreq, u8CurFreq);
        writeTime(pStrTime, u8CurTime);
        writePower(pStrPower, u8CurPower);
    } else {
        writeFreq(pStrFreq, asRecordArray[u8Val-1].valFreq);
        writeTime(pStrTime, asRecordArray[u8Val-1].valTime);
        writePower(pStrPower, asRecordArray[u8Val-1].valPower);
    }

    sprintf((char*)c, "$%s, %s$\n$%s, %s$", pStrFreq, pStrTime, pStrPower, pStrWork);
    sprintf((char*)u8Buf, menuWork.u8MenuString, c);
    return;
}

static void writeWork(char* c, uint8_t u8Val){

    if( MIN_WORK == u8Val){
        sprintf((char*)c, "%s", "Start");
    } else {
        sprintf((char*)c, "Rec. %2d", u8Val);
    }
    *(c+7) = 0;
    return;
}

uint8_t u8IsWorking(void){
    uint8_t flag = FALSE;
    if( enMenuWorking == u8ActiveMenu ){
        flag = TRUE;
    }
    return flag;
}

static void printWorking(uint8_t u8Val){

    bStrCanModify = FALSE;
    char c[40];
    char pStrFreq[8];
    char pStrPower[8];

    writeFreq(pStrFreq, u8CurFreq);
    writePower(pStrPower, u8CurPower);

    sprintf((char*)c, "$%s, %02d:%02d$\n$%s$", pStrFreq, u16TimeRemaining/60, u16TimeRemaining%60, pStrPower);
    sprintf((char*)u8Buf, menuWork.u8MenuString, c);
    if( u16TimeRemaining == 0){
        u8ActiveMenu = enMenuWork;
        u8TempCur = *(menuWork.pU8CurVal);
        menuWork.funcPrintMenu(u8TempCur);
    }
    u16TimeRemaining--;
    return;
}

/* It is function of control for menu. */
uint8_t u8Menu(void) {
    uint8_t ret = 1;

    sMenuType* pThisMenu = asMenuInstances[u8ActiveMenu];
        uint8_t u8PressKey = u8Input();
        switch(u8PressKey){
        case KEY_UP:
            KeyUp(pThisMenu);
            break;
        case KEY_DW:
            KeyDown(pThisMenu);
            break;
        case KEY_SELECT:
            (*pThisMenu).funcKeySelect(pThisMenu);
            break;
        case KEY_CANCEL:
            (*pThisMenu).funcKeyCancel(pThisMenu);
            break;
        case KEY_ESCAPE:
            ret = 0;
            break;
        default:
            if( u8ActiveMenu == enMenuWorking ){
                    (*pThisMenu).funcPrintMenu(*((*pThisMenu).pU8CurVal));
            }
            break;
        }
    return ret;
}

static void hMainKeySelect(sMenuType* const menu) {

    (*(*menu).pU8CurVal) = u8TempCur;
    sMenuType* const sThis = asMenuInstances[u8TempCur];
    if(NULL != (*sThis).funcPrintMenu) {
        (*sThis).funcPrintMenu((*(*sThis).pU8CurVal));
    } else {
        bStrCanModify = 0;
    }
    u8ActiveMenu = (*(*menu).pU8CurVal);
    u8TempCur = (*(*sThis).pU8CurVal);
    return;
}

static void hMainKeyCancel(sMenuType* const menu) {

    u8TempCur = (*(*menu).pU8CurVal);
    if(NULL != (*menu).funcPrintMenu) {
        (*menu).funcPrintMenu(u8TempCur);
    } else {
        bStrCanModify = 0;
    }
    return;
}

static void KeyUp(sMenuType* const struc) {

    UpCur(struc);
    if(NULL != (*struc).funcPrintMenu) {
        (*struc).funcPrintMenu(u8TempCur);
    } else {
        bStrCanModify = 0;
    }
    return;
}

static void KeyDown(sMenuType* const struc) {

    DownCur(struc);
    if(NULL != (*struc).funcPrintMenu) {
        (*struc).funcPrintMenu(u8TempCur);
    } else {
        bStrCanModify = 0;
    }
    return;
}

static void hKeySelectFreq(sMenuType* const struc) {

    (*(*struc).pU8CurVal) = u8TempCur;
    mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal)+1);
    u8TempCur = (*mainMenu.pU8CurVal)+1;
    u8ActiveMenu = (*struc).u8BackMenuIdx;
    return;
}

static void hKeySelectTime(sMenuType* const struc) {

    (*(*struc).pU8CurVal) = u8TempCur;
    mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal)+1);
    u8TempCur = (*mainMenu.pU8CurVal)+1;
    u8ActiveMenu = (*struc).u8BackMenuIdx;
    return;
}

static void hKeySelectPower(sMenuType* const struc) {

    (*(*struc).pU8CurVal) = u8TempCur;
    mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal)+1);
    u8TempCur = (*mainMenu.pU8CurVal)+1;
    u8ActiveMenu = (*struc).u8BackMenuIdx;
    return;
}

static void hKeySelectWork(sMenuType* const struc) {

    (*(*struc).pU8CurVal) = u8TempCur;
    if( (*menuWork.pU8CurVal) == MIN_WORK ) {

        offerRec();
        u8ActiveMenu = enMenuWorking;
        u16TimeRemaining = u16TimeCur2Phys(u8CurTime);
        bStrCanModify = 0;
        sprintf((char*)u8Buf,"$%s$\n$ $", "Launch...");

    } else {

        readRec(u8TempCur-1);
        u8TempCur = 0;
        if(NULL != (*struc).funcPrintMenu) {
            (*struc).funcPrintMenu(u8TempCur);
        } else {
            bStrCanModify = 0;
        }

    }
    return;
}

static void hKeySelectWorking(sMenuType* const struc) {

    (*struc).funcPrintMenu(*((*struc).pU8CurVal));
    return;
}

static void hKeyCancelFreq(sMenuType* const struc) {

    if( u8TempCur != (*(*struc).pU8CurVal)){
        u8TempCur = (*(*struc).pU8CurVal);
        if(NULL != (*struc).funcPrintMenu) {
            (*struc).funcPrintMenu((*(*struc).pU8CurVal));
        } else {
            bStrCanModify = 0;
        }
    } else {
        mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal));
        u8TempCur = (*mainMenu.pU8CurVal);
        u8ActiveMenu = (*struc).u8BackMenuIdx;
    }
    return;
}

static void hKeyCancelTime(sMenuType* const struc) {

    if( u8TempCur != (*(*struc).pU8CurVal)){
        u8TempCur = (*(*struc).pU8CurVal);
        if(NULL != (*struc).funcPrintMenu) {
            (*struc).funcPrintMenu((*(*struc).pU8CurVal));
        } else {
            bStrCanModify = 0;
        }
    } else {
        mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal));
        u8TempCur = (*mainMenu.pU8CurVal);
        u8ActiveMenu = (*struc).u8BackMenuIdx;
    }
    return;
}

static void hKeyCancelPower(sMenuType* const struc) {

    if( u8TempCur != (*(*struc).pU8CurVal)){
        u8TempCur = (*(*struc).pU8CurVal);
        if(NULL != (*struc).funcPrintMenu) {
            (*struc).funcPrintMenu((*(*struc).pU8CurVal));
        } else {
            bStrCanModify = 0;
        }
    } else {
        mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal));
        u8TempCur = (*mainMenu.pU8CurVal);
        u8ActiveMenu = (*struc).u8BackMenuIdx;
    }
    return;
}

static void hKeyCancelWork(sMenuType* const struc) {

    if( u8TempCur != (*(*struc).pU8CurVal)){
        u8TempCur = (*(*struc).pU8CurVal);
        if(NULL != (*struc).funcPrintMenu) {
            (*struc).funcPrintMenu((*(*struc).pU8CurVal));
        } else {
            bStrCanModify = 0;
        }
    } else {
        mainMenu.funcPrintMenu(*(mainMenu.pU8CurVal));
        u8TempCur = (*mainMenu.pU8CurVal);
        u8ActiveMenu = (*struc).u8BackMenuIdx;
    }
    return;
}

static void hKeyCancelWorking(sMenuType* const struc) {

    sMenuType* const sThis = asMenuInstances[(*struc).u8BackMenuIdx];
    (*sThis).funcPrintMenu(*((*sThis).pU8CurVal));
    u8TempCur = (*(*sThis).pU8CurVal);
    u8ActiveMenu = (*struc).u8BackMenuIdx;
    return;
}

static void offerRec(void){

    if( !(u8CurFreq == asRecordArray[0].valFreq && u8CurTime == asRecordArray[0].valTime  && u8CurPower == asRecordArray[0].valPower) ){
        sTypeRecord temp;
        for(uint8_t u8i = 0; (u8i < MAX_WORK); u8i++){
            if(0 != u8i){
                sTypeRecord swap = asRecordArray[u8i];
                asRecordArray[u8i] = temp;
                temp = swap;
            } else {
                temp = asRecordArray[u8i];
                asRecordArray[u8i].valFreq = u8CurFreq;
                asRecordArray[u8i].valTime = u8CurTime;
                asRecordArray[u8i].valPower = u8CurPower;
            }
        }
    }
    return;
}

void checkAllRecs(void){

    for(uint8_t u8i = 0; u8i < MAX_WORK; u8i++){
        if(asRecordArray[u8i].valFreq > MAX_FREQUENCY || asRecordArray[u8i].valTime > MAX_TIME || asRecordArray[u8i].valPower > MAX_POWER){
            asRecordArray[u8i] = defaultRecord;
        }
    }
    return;
}

static void readRec(uint8_t u8Num){

    u8CurFreq = asRecordArray[u8Num].valFreq;
    u8CurTime = asRecordArray[u8Num].valTime;
    u8CurPower = asRecordArray[u8Num].valPower;
    return;
}

#endif
