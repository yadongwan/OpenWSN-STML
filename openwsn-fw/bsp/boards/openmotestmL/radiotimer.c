/**
\brief openmoteSTM32 definition of the "radiotimer" bsp module.

On openmoteSTM32, we use RTC for the radiotimer module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/

#include "stdint.h"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#include "leds.h"
#include "radiotimer.h"
#include "board.h"
#include "main.h"

//#include "rcc.h"
//#include "nvic.h"


//=========================== variables =======================================

enum  radiotimer_irqstatus_enum{
    RADIOTIMER_NONE     = 0x00, //alarm interrupt default status
    RADIOTIMER_OVERFLOW = 0x01, //alarm interrupt caused by overflow
    RADIOTIMER_COMPARE  = 0x02, //alarm interrupt caused by compare
};

typedef struct {
   radiotimer_compare_cbt    overflow_cb;
   radiotimer_compare_cbt    compare_cb;
   uint8_t                   overflowORcompare;//indicate RTC alarm interrupt status
   PORT_RADIOTIMER_WIDTH                  currentSlotPeriod;
} radiotimer_vars_t;

radiotimer_vars_t radiotimer_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================
LPTIM_HandleTypeDef             LptimHandle;
/* Clocks structure declaration */
RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;




static void LSE_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
      ;//Error_Handler();
  }
}

//===== admin

void radiotimer_init() {
   // clear local variables
   memset(&radiotimer_vars,0,sizeof(radiotimer_vars_t));
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
  
  
     /*ssss*/
   LSE_ClockEnable();
   RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
   RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;  //
   HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  LptimHandle.Instance = LPTIM1;
  
  LptimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  //LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_EXTERNAL;
  LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;  
  LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE; 
  LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
  LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
  LptimHandle.Init.Input1Source    = LPTIM_INPUT1SOURCE_GPIO;
  LptimHandle.Init.Input2Source    = LPTIM_INPUT2SOURCE_GPIO;
  
  /* Initialize LPTIM peripheral according to the passed parameters */
  if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
   ;// Error_Handler();
  }
  
  if (HAL_LPTIM_Counter_Start_IT(&LptimHandle, period) != HAL_OK)
  {
    ;//Error_Handler();
  }

    
    radiotimer_vars.currentSlotPeriod = period;
        
    //set radiotimer irpstatus
    radiotimer_vars.overflowORcompare = RADIOTIMER_OVERFLOW;
   
    
    //Configure RTC Alarm interrupt:
   // NVIC_radiotimer();
}

//===== direct access

PORT_RADIOTIMER_WIDTH radiotimer_getValue() {
   // RTC_WaitForSynchro();
    uint32_t counter = HAL_LPTIM_ReadCounter(&LptimHandle);
    //counter = counter << 1;
    return (PORT_RADIOTIMER_WIDTH)counter;
}

void radiotimer_setPeriod(PORT_RADIOTIMER_WIDTH period) {
  
    //period = period >> 1;

  
    
    radiotimer_vars.currentSlotPeriod = period;
    
    //set radiotimer irpstatus
    radiotimer_vars.overflowORcompare = RADIOTIMER_OVERFLOW;
    HAL_LPTIM_Counter_Start_IT(&LptimHandle, period);
}

PORT_RADIOTIMER_WIDTH radiotimer_getPeriod() {
   // RTC_WaitForSynchro();
    uint32_t period = HAL_LPTIM_ReadAutoReload(&LptimHandle);
   // period = period <<1;
    return (PORT_RADIOTIMER_WIDTH)period;
}

//===== compare

void radiotimer_schedule(PORT_RADIOTIMER_WIDTH offset) {
    
   // offset = offset >>1;
    
    HAL_LPTIM_Counter_Start_IT(&LptimHandle, offset);
    
    //set radiotimer irpstatus
    radiotimer_vars.overflowORcompare = RADIOTIMER_COMPARE;
 
}

void radiotimer_cancel() {
  
    HAL_LPTIM_Counter_Start_IT(&LptimHandle, radiotimer_vars.currentSlotPeriod);
   
    //set radiotimer irpstatus
    radiotimer_vars.overflowORcompare = RADIOTIMER_OVERFLOW;
    
}

//===== capture

inline PORT_RADIOTIMER_WIDTH radiotimer_getCapturedTime() {
   
    uint32_t counter =HAL_LPTIM_ReadCounter(&LptimHandle);
//    counter = counter << 1;
    return (PORT_RADIOTIMER_WIDTH)counter;
}

//=========================== private =========================================

//=========================== interrupt handlers ==============================

kick_scheduler_t radiotimer_isr() {
   uint8_t taiv_temp = radiotimer_vars.overflowORcompare;
   switch (taiv_temp) {
      case RADIOTIMER_COMPARE:
         if (radiotimer_vars.compare_cb!=NULL) {
          //  RCC_Wakeup();
            // call the callback
            radiotimer_vars.compare_cb();
            // kick the OS
            return KICK_SCHEDULER;
         }
         break;
      case RADIOTIMER_OVERFLOW: // timer overflows
         if (radiotimer_vars.overflow_cb!=NULL) {
           
            //Wait until last write operation on RTC registers has finished
          //  RTC_WaitForLastTask();                            
            
            //Set the RTC time counter to 0
           HAL_LPTIM_Counter_Stop_IT(&LptimHandle);
          //  RTC_SetCounter(0x00000000);
          //  RTC_WaitForLastTask();
          //  RCC_Wakeup();
            // call the callback
            radiotimer_vars.overflow_cb();
            // kick the OS
            return KICK_SCHEDULER;
         }
         break;
      case RADIOTIMER_NONE:                     // this should not happen
      default:
         while(1);                               // this should not happen
   }
  return DO_NOT_KICK_SCHEDULER;
}


void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
  /* Turn on LED2 */
  radiotimer_isr();
 // BSP_LED_Toggle(LED2);
}
