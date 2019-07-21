/**
\brief openmoteSTM32 definition of the NVIC.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/

#include "stm32l4xx_ll_lib.h"
#include "nvic.h"
//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================

void NVIC_init(void)
{   
  ;
}

//configuration uart interrput
void NVIC_uart(void)
{
//    //Configure NVIC: Preemption Priority = 3 and Sub Priority = 3
//    NVIC_InitTypeDef 	NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 3;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 3;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(5, 3, 3));
    NVIC_EnableIRQ(USART1_IRQn);
}

//configuration spi interrput
void NVIC_spi(void)
{
  #ifdef SPI_IN_INTERRUPT_MODE
//    //Configure NVIC: Preemption Priority = 1 and Sub Priority = 1
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel	                   = SPI1_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority	   = 2;
//    NVIC_InitStructure.NVIC_IRQChannelCmd	           = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(SPI2_IRQn, NVIC_EncodePriority(5, 1, 2));
    NVIC_EnableIRQ(SPI2_IRQn);
  #endif
}

//configuration bsptimer interrput
void NVIC_bsptimer(void)
{
//    //Configure NVIC: Preemption Priority = 2 and Sub Priority = 1
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(5, 2, 1));
    NVIC_EnableIRQ(TIM2_IRQn);
}

//configuration radiotimer interrput
void NVIC_radiotimer(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    //Configure RTC Alarm interrupt:
//    //Configure NVIC: Preemption Priority = 0 and Sub Priority = 1
//    NVIC_InitStructure.NVIC_IRQChannel                    = RTCAlarm_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(LPTIM1_IRQn, NVIC_EncodePriority(5, 1, 0));
    NVIC_EnableIRQ(LPTIM1_IRQn);
}

//configuration radio interrput
void NVIC_radio(void)
{
//     //Configure NVIC: Preemption Priority = 2 and Sub Priority = 0
//    NVIC_InitTypeDef  NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel                     = EXTI15_10_IRQChannel; 
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority          = 1; 
//    NVIC_InitStructure.NVIC_IRQChannelCmd                  = ENABLE; 
//    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(5, 1, 0));
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//configuration of interrupt on openmotestm32
void NVIC_Configuration(void)
{
//    //Set the Vector Table base location
    NVIC_init();
//    //2 bits for Preemption Priority and 2 bits for Sub Priority
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_SetPriorityGrouping(5);
}