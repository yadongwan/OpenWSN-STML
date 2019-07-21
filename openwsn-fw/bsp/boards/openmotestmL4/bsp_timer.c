/**
\brief openmoteSTM32 definition of the "bsp_timer" bsp module.

On openmoteSTM32, we use TIM2 for the bsp_timer module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/
#include "stm32l4xx_ll_lib.h"
#include "string.h"
#include "bsp_timer.h"
#include "board.h"

#include "rcc.h"
#include "nvic.h"

//=========================== defines =========================================

//=========================== variables =======================================

typedef struct {
   bsp_timer_cbt    cb;
   PORT_TIMER_WIDTH last_compare_value;
} bsp_timer_vars_t;

bsp_timer_vars_t bsp_timer_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

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
    
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); 
    
    LL_TIM_InitTypeDef    tim_initstruct;
    LL_TIM_OC_InitTypeDef tim_oc_initstruct;
    //Configure TIM2: Period = 0xffff, prescaler = 1023(32M/(1023+1) = 32.768KHz), CounterMode  = upCounting mode
    
    tim_initstruct.Prescaler         = __LL_TIM_CALC_PSC(SystemCoreClock, 32768);
    tim_initstruct.CounterMode       = LL_TIM_COUNTERMODE_UP;
    tim_initstruct.Autoreload        = 0xFFFF;//__LL_TIM_CALC_ARR(SystemCoreClock, tim_initstruct.Prescaler, 100);
    tim_initstruct.ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;
    tim_initstruct.RepetitionCounter = (uint8_t)0x00;
    LL_TIM_Init(TIM2, &tim_initstruct);
    
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
//    TIM_TimeBaseStructure.TIM_Period        = 0xFFFF;
//    TIM_TimeBaseStructure.TIM_Prescaler     = 1023;;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    //Configure TIM2's out compare mode:  out compare mode = toggle, out compare value = 0 (useless before enable compare interrupt), enable TIM2_CH1
//    TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_Toggle;
//    TIM_OCInitStructure.TIM_Pulse       = 0;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
//    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    
    tim_oc_initstruct.OCMode       = LL_TIM_OCMODE_TOGGLE;
    tim_oc_initstruct.OCState      = LL_TIM_OCSTATE_ENABLE;
    tim_oc_initstruct.OCNState     = LL_TIM_OCSTATE_DISABLE;
    tim_oc_initstruct.CompareValue = 0;
    tim_oc_initstruct.OCPolarity   = LL_TIM_OCPOLARITY_HIGH;
    tim_oc_initstruct.OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;
    tim_oc_initstruct.OCIdleState  = LL_TIM_OCIDLESTATE_LOW;
    tim_oc_initstruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
    LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &tim_oc_initstruct);
    
          
    //enable TIM2
    LL_TIM_EnableCounter(TIM2);
    //disable interrupt
    //bsp_timer_cancel_schedule();
    
//    //Configure NVIC: Preemption Priority = 2 and Sub Priority = 1
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}

/**
\brief Register a callback.

\param cb The function to be called when a compare event happens.
*/
void bsp_timer_set_callback(bsp_timer_cbt cb)
{
   bsp_timer_vars.cb   = cb;
   //enable nvic
   NVIC_bsptimer();
}

/**
\brief Reset the timer.

This function does not stop the timer, it rather resets the value of the
counter, and cancels a possible pending compare event.
*/
void bsp_timer_reset()
{
    // reset compare
    LL_TIM_OC_SetCompareCH1(TIM2,0);
  
    //enable compare interrupt
    LL_TIM_ClearFlag_CC1(TIM2);
    LL_TIM_EnableIT_CC1(TIM2);
    // reset timer
    LL_TIM_SetCounter(TIM2,0);
    
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
   LL_TIM_EnableCounter(TIM2); 
   
   temp_last_compare_value = bsp_timer_vars.last_compare_value;
   
   newCompareValue = bsp_timer_vars.last_compare_value+delayTicks;
   bsp_timer_vars.last_compare_value = newCompareValue;
   
   if (delayTicks < (LL_TIM_GetCounter(TIM2)-temp_last_compare_value)) 
   {
      // setting the interrupt flag triggers an interrupt
//        TIM2->SR |= (u16)TIM_FLAG_CC1; //it can not write one to TIM2 State register.
     LL_TIM_GenerateEvent_CC1(TIM2);
   } 
   else
   {
      // this is the normal case, have timer expire at newCompareValue
     LL_TIM_OC_SetCompareCH1(TIM2,newCompareValue); 
     LL_TIM_ClearFlag_CC1(TIM2);
     LL_TIM_EnableIT_CC1(TIM2);
   }
}

/**
\brief Cancel a running compare.
*/
void bsp_timer_cancel_schedule() 
{
   LL_TIM_OC_SetCompareCH1(TIM2,0);   
   LL_TIM_DisableIT_CC1(TIM2);
}

/**
\brief Return the current value of the timer's counter.

\returns The current value of the timer's counter.
*/
PORT_TIMER_WIDTH bsp_timer_get_currentValue() 
{
   return LL_TIM_GetCounter(TIM2);
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