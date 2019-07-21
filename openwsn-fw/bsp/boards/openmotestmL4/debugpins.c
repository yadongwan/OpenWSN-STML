/**
\brief opemnstm32 definition of the "debugpins" bsp module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
\author Yadong Wan Feb 2019
*/
#include "stm32l4xx_ll_lib.h"
#include "debugpins.h"

//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================

void debugpins_init() {
  
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOC);  
    
    LL_GPIO_InitTypeDef GPIO_InitStructure;
    // Configure PC.0, PC.1 and PC.5 as Output push-pull 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//    
    GPIO_InitStructure.Pin   = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 |LL_GPIO_PIN_5 |LL_GPIO_PIN_8 | LL_GPIO_PIN_12;
    GPIO_InitStructure.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull       = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    
    // Configure PA.5, PA.6 and PA.7 as Output push-pull 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
    GPIO_InitStructure.Pin   = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 |LL_GPIO_PIN_7;
    GPIO_InitStructure.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull       = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_5);// GPIOC->ODR |= 0X0020;      // frame, PC.5
    LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_7);// GPIOA->ODR |= 0X0080;      // slot, PA.7
    LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_5);// GPIOA->ODR |= 0X0020;      // fsm, PA.5
    LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_6);// GPIOA->ODR |= 0X0040;      // task, PA.6
    LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_1);// GPIOC->ODR |= 0X0002;      // isr, PC.1
    LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_0);// GPIOC->ODR |= 0X0001;      // radio, PC.0
   
     debugpins_frame_clr();
     debugpins_slot_clr();
     debugpins_fsm_clr();
     debugpins_task_clr();
     debugpins_isr_clr();
     debugpins_radio_clr();
}

// PC.5
void debugpins_frame_toggle() {
  LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_5);  
  //GPIOC->ODR ^= 0X0020;
}
void debugpins_frame_clr() {
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_5);
  //GPIOC->ODR &= ~0X0020;
}
void debugpins_frame_set() {
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_5);
 // GPIOC->ODR |=  0X0020;
}

// PA.7
void debugpins_slot_toggle() {
   LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_7);  
  //GPIOA->ODR ^=  0X0080;
}
void debugpins_slot_clr() {
  LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_7);
  //GPIOA->ODR &= ~0X0080;
}
void debugpins_slot_set() {
  LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_7);
  //GPIOA->ODR |=  0X0080;
}

// PA.5
void debugpins_fsm_toggle() {
   LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_5);  
  //GPIOA->ODR ^=  0X0020;
}
void debugpins_fsm_clr() {
  LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_5);
  //GPIOA->ODR &= ~0X0020;
}
void debugpins_fsm_set() {
  LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_5);
  //GPIOA->ODR |=  0X0020;
}

// PA.6
void debugpins_task_toggle() {
   LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_6);  
  //GPIOA->ODR ^=  0X0040;
}
void debugpins_task_clr() {
  LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_6);
   //GPIOA->ODR &= ~0X0040;
}
void debugpins_task_set() {
  LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_6);
  //GPIOA->ODR |= 0X0040;
}

// PC.1
void debugpins_isr_toggle() {
   LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_1);  
  //GPIOC->ODR ^=  0X0002;
}
void debugpins_isr_clr() {
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_1);
  //GPIOC->ODR &= ~0X0002;
}
void debugpins_isr_set() {
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_1);
  //GPIOC->ODR |= 0X0002;
}

// PC.0
void debugpins_radio_toggle() {
   LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_0);  
  //GPIOC->ODR ^=  0X0001;
}
void debugpins_radio_clr() {
  LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_0);
  //GPIOC->ODR &= ~0X0001;
}
void debugpins_radio_set() {
  LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_0);
  //GPIOC->ODR |=  0X0001;
}
