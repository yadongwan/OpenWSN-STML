/** 
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_TimeBase/Src/stm32l4xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_ll_lib.h"
#include "stm32l4xx_it.h"
#include "leds.h"
#include "bsp_timer.h"
#include "radiotimer.h"
#include "spi.h"
#include "radio.h"
#include "rtc_timer.h"
#include "uart.h"
#include "debugpins.h"
#include "rcc.h"

/** @addtogroup STM32L4xx_LL_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/
/**
* @brief  This function handles TIM1 update interrupt.
* @param  None
* @retval None
*/
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* Check whether update interrupt is pending */
  if(LL_TIM_IsActiveFlag_UPDATE(TIM1) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_UPDATE(TIM1);
  }
  
  /* TIM1 update interrupt processing */
 // TimerUpdate_Callback();
}


void TIM2_IRQHandler(void)
{
  debugpins_isr_set();
  if(LL_TIM_IsActiveFlag_CC1(TIM2) == 1)
  {
     LL_TIM_ClearFlag_CC1(TIM2);
    //leds_error_toggle();
      bsp_timer_isr();
  }
  debugpins_isr_clr();
}


void LPTIM1_IRQHandler(void)
{
  /* Check whether Autoreload match interrupt is pending */
  if(LL_LPTIM_IsActiveFlag_ARRM(LPTIM1) == 1)
  {
    /* Clear the Autoreload match interrupt flag */
    LL_LPTIM_ClearFLAG_ARRM(LPTIM1);
    
    /* LPTIM1 Autoreload match interrupt processing */
    radiotimer_overflow_isr();

  }
  if(LL_LPTIM_IsActiveFlag_CMPM(LPTIM1) == 1)
  {
    /* Clear the Compare match interrupt flag */
    LL_LPTIM_ClearFLAG_CMPM(LPTIM1);
    
    /* LPTIM1 Compare match interrupt processing */
    radiotimer_compare_isr();
  }
  
}


void SPI2_IRQHandler(void)
{

  // Check RXNE flag value in ISR register 
  if(LL_SPI_IsActiveFlag_RXNE(SPI2))
  {
    spi_isr();
  }
}

void USART1_IRQHandler(void)
{  
    debugpins_isr_set();
    if(LL_USART_IsActiveFlag_RXNE(USART1))
    {
        uart_rx_isr();
    }
  
    if( LL_USART_IsActiveFlag_TXE(USART1))
    { 
        uart_tx_isr(); 
    }
    debugpins_isr_clr();
}


void EXTI15_10_IRQHandler(void)
{
    debugpins_isr_set();
   if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10) != RESET)
   {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
      //leds_sync_toggle();
      radio_isr();
    }
    debugpins_isr_clr();
}
/**
  * @brief  This function handles external lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
//void USER_BUTTON_IRQHANDLER(void)
//{
//  /* Manage Flags */
//  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
//  {
//    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);
//
//    /* Manage code in main.c.*/
//    UserButton_Callback(); 
//  }
//}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
