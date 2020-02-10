#define RCC_APB2ENR *(int*)(0x40021018)
#define IOPCEN (1<<4)
#define GPIOC_CRH *(int*)(0x40011004)
#define OUT_PUSH_PULL_MODE_2MGHR (0xA)
#define GPIOC_CRH_PIN13_OFFSET 19
#define GPIOC_BSRR *(int*)(0x40011010)
#define GPIOC_BRR *(int*)(0x40011014)
#define GPIOC_BSRR_PIN13 (1<<13)

#define FIRST_TIMER 3000000/4
#define SECOND_TIMER 3040000/4
#define THIRD_TIMER 4540000/4
#define FOURTH_TIMER 4580000/4

inline void  GPIO_C_CL_EN()
{
  RCC_APB2ENR |= IOPCEN;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
}

inline void GPIOC_CRH_OUT_PUSH_PULL_MODE_2MGHR()
{
  GPIOC_CRH |= (OUT_PUSH_PULL_MODE_2MGHR<<GPIOC_CRH_PIN13_OFFSET);
}

inline void PC13_SET()
{
  GPIOC_BSRR |= GPIOC_BSRR_PIN13; 
}

inline void PC13_RESET()
{
  GPIOC_BRR |= GPIOC_BSRR_PIN13; 
}

void setup() 
{
  GPIO_C_CL_EN();
  GPIOC_CRH_OUT_PUSH_PULL_MODE_2MGHR();
}

void loop() 
{
     static volatile uint32_t i=1;
     if( 1 == i ) PC13_SET();
     if( FIRST_TIMER == i ) PC13_RESET();
     if( SECOND_TIMER == i ) PC13_SET();
     if( THIRD_TIMER == i ) PC13_RESET();
     if( FOURTH_TIMER == i ) i = 0;
     i++;
}
