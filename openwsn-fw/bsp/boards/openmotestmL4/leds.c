/**
\brief openmoteSTM32 definition of the "leds" bsp module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.

\author Yadong Wan,  Feb. 2019.

*/
#include "stm32l4xx.h"
#include "stm32l4xx_ll_lib.h"
#include "leds.h"

//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

void Delay(void);

//=========================== public ==========================================

void leds_init()
{
    // Enable GPIOC clock
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  LL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// red 
void leds_error_on() 
{
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_6);
  //GPIOC->ODR |= 0X0040;
}
void leds_error_off() 
{
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_6);
  //GPIOC->ODR &= ~0X0040;
}
void leds_error_toggle() 
{
  LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_6);
  // GPIOC->ODR ^= 0X0040;
}
uint8_t leds_error_isOn()
{
  uint8_t bitstatus = 0x00;
  bitstatus = LL_GPIO_IsOutputPinSet(GPIOC,LL_GPIO_PIN_6);
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
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_7);
  // GPIOC->ODR |= 0X0080;
}
void leds_radio_off() 
{
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_7);
  // GPIOC->ODR &= ~0X0080;
}
void leds_radio_toggle() 
{
  LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_7);  
  // GPIOC->ODR ^= 0X0080;
}
uint8_t leds_radio_isOn() 
{
  uint8_t bitstatus = 0x00;
  bitstatus = LL_GPIO_IsOutputPinSet(GPIOC,LL_GPIO_PIN_7);
  return bitstatus;
}

// blue
void leds_sync_on() 
{
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_10);
  // GPIOC->ODR |= 0X0400;
}
void leds_sync_off() 
{
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_10);
  // GPIOC->ODR &= ~0X0400;
}
void leds_sync_toggle()
{
  LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_10);  
//   GPIOC->ODR ^= 0X0400;
}
uint8_t leds_sync_isOn() 
{
  uint8_t bitstatus = 0x00;
  bitstatus = LL_GPIO_IsOutputPinSet(GPIOC,LL_GPIO_PIN_10);
  return bitstatus;
}
// yellow
void    leds_debug_on()
{
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_11);
  // GPIOC->ODR |= 0X0800;  
}

void    leds_debug_off()
{
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_11);
//   GPIOC->ODR &= ~0X0800;  
}

void    leds_debug_toggle()
{
  LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_11);  
  // GPIOC->ODR ^= 0X0800;  
}

uint8_t leds_debug_isOn()
{
  uint8_t bitstatus = 0x00;
  bitstatus = LL_GPIO_IsOutputPinSet(GPIOC,LL_GPIO_PIN_11);
  return bitstatus;
}

void leds_all_on() 
{
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11);
  // GPIOC->ODR |= 0X0CC0;
}
void leds_all_off() 
{
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11);
  // GPIOC->ODR &= ~0X0CC0;
}
void leds_all_toggle() 
{
  LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11);  
  // GPIOC->ODR ^= 0X0CC0;
}

void leds_circular_shift() 
{
     LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_6);  
     Delay();
     LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_7);  
     Delay();
      LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_10);  
     Delay();
      LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_11);  
     Delay();
      LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_6);  
     Delay();
      LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_7);  
     Delay();
      LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_10);  
     Delay();
      LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_11);  
     Delay();
}

void leds_increment()
{
  
}

//=========================== private =========================================

void Delay(void)    //delay
{
  volatile uint32_t ik;
  //for(uint16_t jk=0;jk<20;jk++)
    for(ik=0;ik<0x7ffff;ik++) 
      ;
}