#define GPIOC_BSRR *(int*)(0x40011010)
#define GPIOC_BRR *(int*)(0x40011014)
#define GPIOC_BSRR_PIN13 (1<<13)

#define FIRST_TIMER 3000000/4

inline void PC13_SET()
{
  GPIOC_BSRR |= GPIOC_BSRR_PIN13; 
}

inline void PC13_RESET()
{
  GPIOC_BRR |= GPIOC_BSRR_PIN13; 
}

uint8_t volatile flag=0;

void EXTI3_IRQHandler(void)
{
  flag |= (0x1<<3);
  EXTI->PR |= flag; //Очищаем флаг
}

void setup()
{

  GPIO_InitTypeDef PORT;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

// Настроим ноги на выход
  PORT.Pin = (GPIO_PIN_3);
  PORT.Mode = GPIO_MODE_INPUT;
  PORT.Pull = GPIO_PULLUP;
  PORT.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &PORT);


  PORT.Pin = (GPIO_PIN_13);
  PORT.Mode = GPIO_MODE_OUTPUT_PP;
  PORT.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &PORT);

  attachInterrupt(PA3, EXTI3_IRQHandler, FALLING);
  
  PC13_SET();
}

void loop()
{
  if( 0 != flag)
  {
    PC13_RESET();
    for(volatile uint32_t i = 0; i < FIRST_TIMER*10; i++);
    PC13_SET();
    flag = 0;
  }
}
