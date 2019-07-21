/**
\brief openmoteSTM32 definition of the "leds" bsp module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
\author Yadong Wan ,June, 2018
*/
#include "stm32l4xx_hal.h"
#include "leds.h"

//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

void Delay(void);

//=========================== public ==========================================

void leds_init(void)
{
  static GPIO_InitTypeDef  GPIO_InitStruct;
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure IOs in output push-pull mode to drive external LEDs */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
}

// red
void leds_error_on() 
{
  GPIOC->ODR |= 0X0040;
}
void leds_error_off() 
{
   GPIOC->ODR &= ~0X0040;
}
void leds_error_toggle() 
{
   GPIOC->ODR ^= 0X0040;
}
uint8_t leds_error_isOn()
{
  uint8_t bitstatus = 0x00;
  if ((GPIOC->ODR & 0X0040) != (uint32_t)0)
  {
    bitstatus = 0x01;
  }
  else
  {
    bitstatus = 0x00;
  }
  return bitstatus;
}

void    leds_error_blink()
{
  for(int i=0;i<16;i++) {
    leds_error_toggle();
    Delay();
  }
  
}

// green
void    leds_radio_on() 
{
   GPIOC->ODR |= 0X0080;
}
void leds_radio_off() 
{
   GPIOC->ODR &= ~0X0080;
}
void leds_radio_toggle() 
{
   GPIOC->ODR ^= 0X0080;
}
uint8_t leds_radio_isOn() 
{
  uint8_t bitstatus = 0x00;
  if ((GPIOC->ODR & 0X0080) != (uint32_t)0)
  {
    bitstatus = 0x01;
  }
  else
  {
    bitstatus = 0x00;
  }
  return bitstatus;
}

// blue
void leds_sync_on() 
{
   GPIOC->ODR |= 0X0400;
}
void leds_sync_off() 
{
   GPIOC->ODR &= ~0X0400;
}
void leds_sync_toggle()
{
   GPIOC->ODR ^= 0X0400;
}
uint8_t leds_sync_isOn() 
{
  uint8_t bitstatus = 0x00;
  if ((GPIOC->ODR & 0X0400) != (uint32_t)0)
  {
    bitstatus = 0x01;
  }
  else
  {
    bitstatus = 0x00;
  }
  return bitstatus;
}
// yellow
void    leds_debug_on()
{
   GPIOC->ODR |= 0X0800;  
}

void    leds_debug_off()
{
   GPIOC->ODR &= ~0X0800;  
}

void    leds_debug_toggle()
{
   GPIOC->ODR ^= 0X0800;  
}

uint8_t leds_debug_isOn()
{
  uint8_t bitstatus = 0x00;
  if ((GPIOC->ODR & 0X0800) != (uint32_t)0)
  {
    bitstatus = 0x01;
  }
  else
  {
    bitstatus = 0x00;
  }
  return bitstatus;
}

void leds_all_on() 
{
   GPIOC->ODR |= 0X0CC0;
}
void leds_all_off() 
{
   GPIOC->ODR &= ~0X0CC0;
}
void leds_all_toggle() 
{
   GPIOC->ODR ^= 0X0CC0;
}

void leds_circular_shift() 
{
     GPIOC->ODR ^= 0X0040;
     Delay();
     GPIOC->ODR ^= 0X0040;
     Delay();
     GPIOC->ODR ^= 0X0080;
     Delay();
     GPIOC->ODR ^= 0X0080;
     Delay();
     GPIOC->ODR ^= 0X0400;
     Delay();
     GPIOC->ODR ^= 0X0400;
     Delay();
     GPIOC->ODR ^= 0X0800;
     Delay();
     GPIOC->ODR ^= 0X0800;
     Delay();
}

void leds_increment()
{
  
}

//=========================== private =========================================

void Delay(void)    //delay
{
  volatile unsigned int i=0,j=0;
  for(i =0;i<0xf;i++)
    for(j=0;j<0xffff;j++)
      ;
  //HAL_Delay(100);
}