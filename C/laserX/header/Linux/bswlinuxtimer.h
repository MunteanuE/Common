#if !defined(BSWLINUXTIMER_H) && defined(LINUX)
#define BSWLINUXTIMER_H
#define TRUE 1
#define MS_TICK_TIME 10UL

extern uint8_t u8IsMsTimeout(uint8_t f100Ms);

#endif
