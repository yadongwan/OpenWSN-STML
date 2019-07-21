/**
\brief openmoteSTM32 definition of the "bsp_timer" bsp module.

On openmoteSTM32, we use TIM2 for the bsp_timer module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/
#include "stm32l4xx_hal.h"
#include "string.h"
#include "bsp_timer.h"
#include "board.h"

//#include "rcc.h"
//#include "nvic.h"
static void Error_Handler(void);
//=========================== defines =========================================

//=========================== variables =======================================

typedef struct {
   bsp_timer_cbt    cb;
   PORT_TIMER_WIDTH last_compare_value;
} bsp_timer_vars_t;

bsp_timer_vars_t bsp_timer_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================
TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef sConfig;
/**
\brief Initialize this module.

This functions starts the timer, i.e. the counter increments, but doesn't set
any compare registers, so no interrupt will fire.
*/
void bsp_timer_init() 
{

    // clear local variables
    memset(&bsp_timer_vars,0,sizeof(bsp_timer_vars_t));
    
    //Configure TIM2, Clock
   __HAL_RCC_TIM2_CLK_ENABLE();
    
   uint32_t uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;
   TimHandle.Instance = TIM1;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = 0xFFFF;
  TimHandle.Init.Prescaler         = 2441;//uwPrescalerValue*32; // 1K clock
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
    //Configure TIM2: Period = 0xffff, prescaler = 1023(32M/(1023+1) = 32.768KHz), CounterMode  = upCounting mode
  if(HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Configure the Output Compare channels #########################################*/ 
  /* Common configuration for all channels */
  sConfig.OCMode     = TIM_OCMODE_ACTIVE;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

  /* Set the pulse (delay1)  value for channel 1 */
  sConfig.Pulse = 0;  
  if(HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /*##-4- Start signals generation #######################################*/ 
  /* Start channel 1 in Output compare mode */
  if(HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}

/**
\brief Register a callback.

\param cb The function to be called when a compare event happens.
*/
void bsp_timer_set_callback(bsp_timer_cbt cb)
{
   bsp_timer_vars.cb   = cb;
   //enable nvic
  // NVIC_bsptimer(); // nvic.c
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

/**
\brief Reset the timer.

This function does not stop the timer, it rather resets the value of the
counter, and cancels a possible pending compare event.
*/
void bsp_timer_reset()
{
    // reset compare
   __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_1, 0);
    //enable compare interrupt
    __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC1);
    HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_1);
    //TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    
    // reset timer
    //TIM_SetCounter(TIM2,0);
    __HAL_TIM_SET_COUNTER(&TimHandle,0);
    // record last timer compare value
    bsp_timer_vars.last_compare_value =  0;
}

/**
\brief Schedule the callback to be called in some specified time.

The delay is expressed relative to the last compare event. It doesn't matter
how long it took to call this function after the last compare, the timer will
expire precisely delayTicks after the last one.

The only possible problem is that it took so long to call this function that
the delay specified is shorter than the time already elapsed since the last
compare. In that case, this function triggers the interrupt to fire right away.

This means that the interrupt may fire a bit off, but this inaccuracy does not
propagate to subsequent timers.

\param delayTicks Number of ticks before the timer expired, relative to the
                  last compare event.
*/
void bsp_timer_scheduleIn(PORT_TIMER_WIDTH delayTicks) 
{
   PORT_TIMER_WIDTH newCompareValue;
   PORT_TIMER_WIDTH temp_last_compare_value;
   //enable it if not enabled.
   //TIM_Cmd(TIM2, ENABLE); 
   __HAL_TIM_ENABLE(&TimHandle);
   temp_last_compare_value = bsp_timer_vars.last_compare_value;
   
   newCompareValue = bsp_timer_vars.last_compare_value+delayTicks;
   bsp_timer_vars.last_compare_value = newCompareValue;
   
   if (delayTicks < (__HAL_TIM_GET_COUNTER(&TimHandle)-temp_last_compare_value)) 
   {
      // setting the interrupt flag triggers an interrupt
//        TIM2->SR |= (u16)TIM_FLAG_CC1; //it can not write one to TIM2 State register.
    // TIM_GenerateEvent(TIM2,TIM_EventSource_CC1);
   HAL_TIM_GenerateEvent(&TimHandle,TIM_EVENTSOURCE_CC1);
   } 
   else
   {
      // this is the normal case, have timer expire at newCompareValue
      //TIM_SetCompare1(TIM2,newCompareValue);
      __HAL_TIM_SET_COMPARE(&TimHandle,TIM_CHANNEL_1,newCompareValue);
       __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC1);
      HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_1);
    //  TIM_ClearFlag(TIM2, TIM_FLAG_CC1);
    //  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
   }
}

/**
\brief Cancel a running compare.
*/
void bsp_timer_cancel_schedule() 
{
    //TIM_SetCompare1(TIM2,0);
    //TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE); 
    __HAL_TIM_SET_COMPARE(&TimHandle,TIM_CHANNEL_1,0);
    __HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC1);
    HAL_TIM_OC_Stop_IT(&TimHandle, TIM_CHANNEL_1);
}

/**
\brief Return the current value of the timer's counter.

\returns The current value of the timer's counter.
*/
PORT_TIMER_WIDTH bsp_timer_get_currentValue() 
{
   return __HAL_TIM_GET_COUNTER(&TimHandle);//TIM_GetCounter(TIM2);
}

//=========================== private =========================================

//=========================== interrupt handlers ==============================

kick_scheduler_t bsp_timer_isr()
{
   // call the callback
   bsp_timer_vars.cb();
   // kick the OS
   return KICK_SCHEDULER;
}

/**
  * @brief  Output Compare callback in non blocking mode 
  * @param  htim : TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  
  /* TIM1_CH1 toggling with frequency = 195.307 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {bsp_timer_isr();
//HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    /* Set the Capture Compare Register value */
  //  __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_1, 0);
  }
  

}

static void Error_Handler(void)
{
  while (1)
  {
  }
}