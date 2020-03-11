#ifndef MENU_H
#include "menu.h"
#endif

#ifndef BSWNVM_H
#define BSWNVM_H
extern void initNVM(void);
extern void writeNVM(sTypeRecord*);
extern sTypeRecord* readNVM(void);
#endif
