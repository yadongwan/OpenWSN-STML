/**
\brief openmoteSTM32 definition of the "uart" bsp module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/

#include "stm32l4xx_ll_lib.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "uart.h"
#include "leds.h"

#include "rcc.h"
#include "nvic.h"

//=========================== defines =========================================

//=========================== variables =======================================

typedef struct {
   uart_tx_cbt txCb;
   uart_rx_cbt rxCb;
   uint8_t     startOrend;
   uint8_t     flagByte;
   bool        flag;
} uart_vars_t;

uart_vars_t uart_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

void uart_init() 
{
    // reset local variables
    memset(&uart_vars,0,sizeof(uart_vars_t));
    
    //when this value is 0, we are send the first data
    uart_vars.startOrend = 0;
    //flag byte for start byte and end byte
    uart_vars.flagByte = 0x7E;
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);  
    // Configure PA.9 as alternate function push-pull
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);
 
    
    //Configure PA.10 as input floating 
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
    
    
    LL_USART_InitTypeDef usart_initstruct;
    // Enable USART1 and GPIOC clock

   
    //configure USART1 :Baud rate = 115200bit/s, data bit = 8bit, stop bit = 1, no parity, no flow control enable Tx and Rx 
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
    usart_initstruct.BaudRate            = 115200;
    usart_initstruct.DataWidth           = LL_USART_DATAWIDTH_8B;
    usart_initstruct.StopBits            = LL_USART_STOPBITS_1;
    usart_initstruct.Parity              = LL_USART_PARITY_NONE;
    usart_initstruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
    usart_initstruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    usart_initstruct.OverSampling        = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &usart_initstruct);

    // Enable USART 
    LL_USART_Enable(USART1);
    //Polling USART initialisation
    while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
    { 
    }
    if(LL_USART_IsActiveFlag_RXNE(USART1)==1)
     LL_USART_ReceiveData8(USART1);
//
//  /* Enable RXNE and Error interrupts */
//  LL_USART_EnableIT_RXNE(USARTx_INSTANCE);
//  LL_USART_EnableIT_ERROR(USARTx_INSTANCE);
    
 
}

void uart_setCallbacks(uart_tx_cbt txCb, uart_rx_cbt rxCb) 
{
    uart_vars.txCb = txCb;
    uart_vars.rxCb = rxCb;
    
    //enable nvic uart.
     NVIC_uart();
}

void uart_enableInterrupts()
{
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    //LL_USART_EnableIT_TXE(USART1);
    LL_USART_EnableIT_RXNE(USART1);
}

void uart_disableInterrupts()
{
  LL_USART_DisableIT_RXNE(USART1);
  LL_USART_DisableIT_TXE(USART1);
}

void uart_clearRxInterrupts()
{
   // USART_ClearFlag(USART1,USART_FLAG_RXNE);
   // no rxne clear function
}

void uart_clearTxInterrupts()
{
   // USART_ClearFlag(USART1,USART_FLAG_TXE);
  //no txe clear function
}

void uart_writeByte(uint8_t byteToWrite)
{ 
    
    LL_USART_TransmitData8(USART1,(uint16_t)byteToWrite);
    while(LL_USART_IsActiveFlag_TXE(USART1) == RESET);
   // LL_USART_EnableIT_TXE(USART1);
      //start or end byte?
    if(byteToWrite == uart_vars.flagByte){
      uart_vars.startOrend = (uart_vars.startOrend == 0)?1:0;
      //start byte
      if(uart_vars.startOrend == 1)
        LL_USART_EnableIT_TXE(USART1);
      else
        LL_USART_DisableIT_TXE(USART1);
    }
    
}

uint8_t uart_readByte()
{
    uint16_t temp;
    temp = LL_USART_ReceiveData8(USART1);
    return (uint8_t)temp;
}

//=========================== interrupt handlers ==============================

kick_scheduler_t uart_tx_isr() 
{
    uart_vars.txCb();
    return DO_NOT_KICK_SCHEDULER;
}

kick_scheduler_t uart_rx_isr() 
{
    uart_vars.rxCb();
    return DO_NOT_KICK_SCHEDULER;
}