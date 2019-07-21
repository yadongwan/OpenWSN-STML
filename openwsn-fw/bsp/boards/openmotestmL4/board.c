/**
\brief openmoteSTM32 definition of the "board" bsp module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
\author Yadong Wan, Feb 2019
*/

#include "stm32l4xx_ll_lib.h"
#include "board.h"
// bsp modules
#include "leds.h"
#include "uart.h"
#include "spi.h"
#include "bsp_timer.h"
#include "radio.h"
#include "radiotimer.h"
#include "rcc.h"
#include "nvic.h"
#include "debugpins.h"
#include "opentimers.h"
#include "gpio.h"

//=========================== variable ========================================

//=========================== main ============================================

extern int mote_main(void);
int main() {
   return mote_main();
}

//=========================== public ==========================================

void board_init()
{
    RCC_Configuration();//Configure rcc
    NVIC_Configuration();//configure NVIC and Vector Table
    
    //configure ALL GPIO to AIN to get lowest power
    //GPIO_Config_ALL_AIN();
    //configuration GPIO to measure the time from sleep to 72MHz
    GPIO_Configuration();
    
    LL_GPIO_InitTypeDef  GPIO_InitStructure;
    //enable GPIOB, Clock
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    //Configure PB.01 as SLP_TR pin of RF
    GPIO_InitStructure.Pin   = LL_GPIO_PIN_1;
    GPIO_InitStructure.Mode  = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    //Configure PB.11 as RST pin of RF
    GPIO_InitStructure.Pin   = LL_GPIO_PIN_11;
    GPIO_InitStructure.Mode  = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //set /RST pin high(never reset)
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_11);
    
    // Configure PB.10 as input floating (EXTI Line10)
    GPIO_InitStructure.Pin   = LL_GPIO_PIN_10;
    GPIO_InitStructure.Mode  = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull  = LL_GPIO_PULL_NO;    
    LL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10);
   // GPIOB->ODR |= 0X0400;//set low
    
    //Connect EXTI Line10 to PB.10
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE10); 
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
    
    //Configures EXTI line 10 to generate an interrupt on rising edge
    LL_EXTI_InitTypeDef  EXTI_InitStructure; 
    EXTI_InitStructure.Line_0_31    = LL_EXTI_LINE_10;
    EXTI_InitStructure.Line_32_63   = LL_EXTI_LINE_NONE;
    EXTI_InitStructure.Mode         = LL_EXTI_MODE_IT; 
    EXTI_InitStructure.Trigger      = LL_EXTI_TRIGGER_RISING;
    EXTI_InitStructure.LineCommand  = ENABLE; 
    LL_EXTI_Init(&EXTI_InitStructure);
    
    // initialize board
    leds_init();
    uart_init();
    spi_init();
    bsp_timer_init();
    radio_init();
    radiotimer_init();
    debugpins_init();
    //enable nvic for the radio
    NVIC_radio();
}

void board_sleep() {
  
#ifdef DEBUG_RUN_MODE
    // nothing need to do
#endif
  
#ifdef DEBUG_SLEEP_MODE
//    DBGMCU_Config(DBGMCU_SLEEP, ENABLE);
//    // Enable PWR and BKP clock
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//    // Desable the SRAM and FLITF clock in sleep mode
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);
//    // enter sleep mode
//    __WFI();
#endif
  
#ifdef DEBUG_STOP_MODE
//    uint16_t sleepTime = radiotimer_getPeriod() - radiotimer_getCapturedTime();
//    
//    DBGMCU_Config(DBGMCU_STOP, ENABLE);
//    // Enable PWR and BKP clock
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//    // Desable the SRAM and FLITF clock in Stop mode
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);
//    // enter stop mode
//    PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);
//    
//    if(sleepTime > 0)
//    opentimers_sleepTimeCompesation(sleepTime);
#endif
}



void board_reset(){
//  NVIC_GenerateSystemReset();
}


 

