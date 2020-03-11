#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif


#if !defined(TERMIOS_H) && defined(LINUX)
#include <termios.h>
#define TERMIOS_H
#endif

#if !defined(PTHREAD_H) && defined(LINUX)
#include <pthread.h>
#define PTHREAD_H
#endif

#if !defined(BSWLINUXMENU_H) && defined(LINUX)
#include "bswlinuxmenu.h"

inline static void initTermios(unsigned int);
inline static void resetTermios(void);
inline static void* getch_(void*);
void printString(const char*, const char*);
void printChar(const char);

extern uint8_t getchar(void);
extern int putchar ( int character );
extern int32_t printf(const char *, ...);

static struct termios sOld, sNew;
uint8_t u8Ch;
uint8_t fRisen = 0;

inline static void initTermios(unsigned int echo){

  tcgetattr(0, &sOld);
  sNew = sOld;
  sNew.c_lflag &= ~ICANON;
  sNew.c_lflag &= echo ? ECHO : ~ECHO;
  sNew.c_cc[VTIME] = 50;
  tcsetattr(0, TCSANOW, &sNew);
  return;
}

inline static void resetTermios(void){

  tcsetattr(0, TCSANOW, &sOld);
  return;
}

inline static void* rise_(void* arg){

    pthread_t thr;
    while(1){
        if(!fRisen){
            fRisen = 1;
            pthread_create(&thr, NULL, getch_, (void *)arg);
        }
    }
    return NULL;
}

inline static void* getch_(void* arg){

  initTermios(*((uint8_t*)arg));
  u8Ch = getchar();
  resetTermios();
  fRisen = 0;
  return NULL;
}

uint8_t u8Getch(void){

    pthread_t thr;
    const uint8_t null = 0;
    const uint8_t* arg = &null;

    static uint8_t fInit = 0;
    if(!fInit){
        pthread_create(&thr, NULL, rise_, (void *)arg);
        fInit = 1;
    }
    uint8_t ret = u8Ch;
    u8Ch = 0;
    return ret;
}

void rteLCDMain(const char u8c){

    putchar((int)u8c);
    return;
}

#endif
