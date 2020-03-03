#if !defined(CONIO_H) && defined(WINDOWS)
#include <conio.h>
#define CONIO_H
#endif


#if !defined(BSWWINDOWSMENU_H) && defined(WINDOWS)
#define BSWWINDOWSMENU_H

/*KEY_SELECT is right key.
  KEY_CANCEL is left key.*/
#define KEY_UP      77U
#define KEY_DW      75U
#define KEY_SELECT  13U
#define KEY_CANCEL  27U
#define KEY_ESCAPE 3U

extern void printString(const char*, const uint8_t*);

#endif
