#if !defined(BSWLINUXMENU_H) && defined(LINUX)
#define BSWLINUXMENU_H

/*KEY_SELECT is right key.
  KEY_CANCEL is left key.*/
#define KEY_UP      65U
#define KEY_DW      66U
#define KEY_SELECT  67U
#define KEY_CANCEL  68U
#define KEY_ESCAPE 0x65

extern uint8_t u8Getch(void);
extern void rteLCDMain(const char);

#endif
