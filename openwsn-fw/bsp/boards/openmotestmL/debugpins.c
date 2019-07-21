/**
\brief opemnstm32 definition of the "debugpins" bsp module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
*/
#include "stm32l4xx_hal.h"
#include "debugpins.h"

//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================

void debugpins_init() 
{
   static GPIO_InitTypeDef  GPIO_InitStruct;
  //portC and portA as debug pin
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
 
  // Configure PC.0, PC.1 and PC.5 as Output push-pull 
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;//|GPIO_PIN_8|GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
  // Configure PA.5, PA.6 and PA.7 as Output push-pull 
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);    
    
     GPIOC->ODR |= 0X0020;      // frame, PC.5
     GPIOA->ODR |= 0X0080;      // slot, PA.7
     GPIOA->ODR |= 0X0020;      // fsm, PA.5
     GPIOA->ODR |= 0X0040;      // task, PA.6
     GPIOC->ODR |= 0X0002;      // isr, PC.1
     GPIOC->ODR |= 0X0001;      // radio, PC.0
   
     debugpins_frame_clr();
     debugpins_slot_clr();
     debugpins_fsm_clr();
     debugpins_task_clr();
     debugpins_isr_clr();
     debugpins_radio_clr();
}

// PC.5
void debugpins_frame_toggle(void) {
  GPIOC->ODR ^= 0X0020;
}
void debugpins_frame_clr(void) {
  GPIOC->ODR &= ~0X0020;
}
void debugpins_frame_set(void) {
  GPIOC->ODR |=  0X0020;
}

// PA.7
void debugpins_slot_toggle(void) {
  GPIOA->ODR ^=  0X0080;
}
void debugpins_slot_clr(void) {
  GPIOA->ODR &= ~0X0080;
}
void debugpins_slot_set(void) {
  GPIOA->ODR |=  0X0080;
}

// PA.5
void debugpins_fsm_toggle() {
  GPIOA->ODR ^=  0X0020;
}
void debugpins_fsm_clr(void) {
  GPIOA->ODR &= ~0X0020;
}
void debugpins_fsm_set(void) {
  GPIOA->ODR |=  0X0020;
}

// PA.6
void debugpins_task_toggle(void) {
  GPIOA->ODR ^=  0X0040;
}
void debugpins_task_clr(void) {
  GPIOA->ODR &= ~0X0040;
}
void debugpins_task_set(void) {
  GPIOA->ODR |= 0X0040;
}

// PC.1
void debugpins_isr_toggle(void) {
  GPIOC->ODR ^=  0X0002;
}
void debugpins_isr_clr(void) {
  GPIOC->ODR &= ~0X0002;
}
void debugpins_isr_set(void) {
  GPIOC->ODR |= 0X0002;
}

// PC.0
void debugpins_radio_toggle(void) {
  GPIOC->ODR ^=  0X0001;
}
void debugpins_radio_clr(void) {
  GPIOC->ODR &= ~0X0001;
}
void debugpins_radio_set(void) {
  GPIOC->ODR |=  0X0001;
}
