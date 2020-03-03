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
inline static void* s8Getch_(void*);
void printString(const char*, const char*);
void printChar(const char);

extern uint8_t getchar(void);
extern int putchar ( int character );
extern int32_t printf(const char *, ...);

static struct termios stOld, stNew;
pthread_mutex_t lock;
uint8_t u8Ch;
uint8_t fRis = 0;

inline static void initTermios(unsigned int echo){

  tcgetattr(0, &stOld);
  stNew = stOld;
  stNew.c_lflag &= ~ICANON;
  stNew.c_lflag &= echo ? ECHO : ~ECHO;
  stNew.c_cc[VTIME] = 50;
  tcsetattr(0, TCSANOW, &stNew);
  return;
}

inline static void resetTermios(void){

  tcsetattr(0, TCSANOW, &stOld);
  return;
}

inline static void* rise_(void* arg){

    pthread_t thr;
    while(1){
        if(!fRis){
            fRis = 1;
            pthread_create(&thr, NULL, s8Getch_, (void *)arg);
        }
    }
    return NULL;
}

inline static void* s8Getch_(void* arg){

  initTermios(0);
  u8Ch = getchar();
  resetTermios();
  fRis = 0;
  return NULL;
}

uint8_t getch(void){

    pthread_t thr;
    uint8_t arg = 0;

    static uint8_t fInit = 0;
    if(!fInit){
        pthread_create(&thr, NULL, rise_, (void *)arg);
        fInit = 1;
    }
    uint8_t ret = u8Ch;
    u8Ch = 0;
    return ret;
}

void printChar(const char u8c){

    putchar((int)u8c);
    return;
}

#endif
