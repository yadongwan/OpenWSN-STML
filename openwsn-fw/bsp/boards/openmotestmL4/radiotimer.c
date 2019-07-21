/**
\brief openmoteSTM32 definition of the "radiotimer" bsp module.

On openmoteSTM32, we use RTC for the radiotimer module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
\author Yadong Wan Feb. 2019
*/

#include "stdint.h"

#include "stm32l4xx_ll_lib.h"
#include "leds.h"
#include "radiotimer.h"
#include "board.h"

#include "rcc.h"
#include "nvic.h"


//=========================== variables =======================================

enum  radiotimer_irqstatus_enum{
    RADIOTIMER_NONE     = 0x00, //alarm interrupt default status
    RADIOTIMER_OVERFLOW = 0x01, //alarm interrupt caused by overflow
    RADIOTIMER_COMPARE  = 0x02, //alarm interrupt caused by compare
};

typedef struct {
   radiotimer_compare_cbt    overflow_cb;
   radiotimer_compare_cbt    compare_cb;
} radiotimer_vars_t;

radiotimer_vars_t radiotimer_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

//===== admin

void radiotimer_init() {
   // clear local variables
   memset(&radiotimer_vars,0,sizeof(radiotimer_vars_t));
      //use lptim1 LSE
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
   LL_PWR_EnableBkUpAccess();
   while(LL_PWR_IsEnabledBkUpAccess()!=1)
     ;
   LL_RCC_LSE_Enable();
   while(LL_RCC_LSE_IsReady() != 1) 
     ;
   
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1); 
   LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSE);
}

void radiotimer_setOverflowCb(radiotimer_compare_cbt cb) {
   radiotimer_vars.overflow_cb    = cb;
}

void radiotimer_setCompareCb(radiotimer_compare_cbt cb) {
   radiotimer_vars.compare_cb     = cb;
}

void radiotimer_setStartFrameCb(radiotimer_capture_cbt cb) {
   while(1);
}

void radiotimer_setEndFrameCb(radiotimer_capture_cbt cb) {
   while(1);
}

void radiotimer_start(PORT_RADIOTIMER_WIDTH period) {
  /*****************************/
  /* LPTIM1 configuration      */
  /*****************************/
  LL_LPTIM_InitTypeDef    lptim_initstruct;
  /* Set fields of initialization structure */
  lptim_initstruct.ClockSource = LL_LPTIM_CLK_SOURCE_INTERNAL;
  lptim_initstruct.Prescaler   = LL_LPTIM_PRESCALER_DIV1;
  lptim_initstruct.Waveform    = LL_LPTIM_OUTPUT_WAVEFORM_PWM;
  lptim_initstruct.Polarity    = LL_LPTIM_OUTPUT_POLARITY_REGULAR;
  
  LL_LPTIM_Init(LPTIM1, &lptim_initstruct);
  
  /* The counter is incremented following each valid clock pulse on the LPTIM external Input1 */
  LL_LPTIM_SetCounterMode(LPTIM1, LL_LPTIM_COUNTER_MODE_INTERNAL);
  LL_LPTIM_Enable(LPTIM1);

  LPTIM1->CNT = 0;
  LL_LPTIM_SetAutoReload(LPTIM1, period);
  LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS);
  LL_LPTIM_EnableIT_ARRM(LPTIM1);
  //LL_LPTIM_EnableIT_CMPM(LPTIM1);  
    //Configure RTC Alarm interrupt:
  NVIC_radiotimer();
}

//===== direct access

PORT_RADIOTIMER_WIDTH radiotimer_getValue() {
    uint32_t counter = LL_LPTIM_GetCounter(LPTIM1);
    return (PORT_RADIOTIMER_WIDTH)counter;
}

void radiotimer_setPeriod(PORT_RADIOTIMER_WIDTH period) {
  LL_LPTIM_DisableIT_ARRM(LPTIM1);
  LL_LPTIM_SetAutoReload(LPTIM1, period);
  LL_LPTIM_EnableIT_ARRM(LPTIM1);
}

PORT_RADIOTIMER_WIDTH radiotimer_getPeriod() {
   
    uint32_t period = LL_LPTIM_GetAutoReload(LPTIM1);
    return (PORT_RADIOTIMER_WIDTH)period;
}

//===== compare

void radiotimer_schedule(PORT_RADIOTIMER_WIDTH offset) {
    LL_LPTIM_DisableIT_CMPM(LPTIM1); 
    LL_LPTIM_SetCompare(LPTIM1,offset);
    LL_LPTIM_EnableIT_CMPM(LPTIM1);  
}

void radiotimer_cancel() {
    LL_LPTIM_DisableIT_CMPM(LPTIM1); 
    //LL_LPTIM_SetCompare(LPTIM1,0xffff);
  //  LL_LPTIM_EnableIT_CMPM(LPTIM1);  
}

//===== capture

 PORT_RADIOTIMER_WIDTH radiotimer_getCapturedTime() {
    uint32_t counter = LL_LPTIM_GetCounter(LPTIM1);
    return (PORT_RADIOTIMER_WIDTH)counter;
}

//=========================== private =========================================

//=========================== interrupt handlers ==============================

kick_scheduler_t radiotimer_overflow_isr() {

   if (radiotimer_vars.overflow_cb!=NULL) {
      LPTIM1->CNT = 0;
      radiotimer_vars.overflow_cb();
      return KICK_SCHEDULER;
   } 
  return DO_NOT_KICK_SCHEDULER;
}

kick_scheduler_t radiotimer_compare_isr() {   
  if (radiotimer_vars.compare_cb!=NULL) {
      radiotimer_vars.compare_cb();
      return KICK_SCHEDULER;
   }
  return DO_NOT_KICK_SCHEDULER;  
}
