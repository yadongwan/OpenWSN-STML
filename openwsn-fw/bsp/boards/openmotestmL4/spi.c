/**
\brief openmoteSTM32 definition of the "spi" bsp module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
\author Yadong Wan,  February 2019.

*/
#include "stm32l4xx_ll_lib.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "spi.h"
#include "leds.h"

#include "rcc.h"
#include "nvic.h"

//=========================== defines =========================================

//=========================== variables =======================================

typedef struct {
   // information about the current transaction
   uint8_t*        pNextTxByte;
   uint8_t         numTxedBytes;
   uint8_t         txBytesLeft;
   spi_return_t    returnType;
   uint8_t*        pNextRxByte;
   uint8_t         maxRxBytes;
   spi_first_t     isFirst;
   spi_last_t      isLast;
   // state of the module
   uint8_t         busy;
#ifdef SPI_IN_INTERRUPT_MODE
   // callback when module done
   spi_cbt         callback;
#endif
} spi_vars_t;

spi_vars_t spi_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

void spi_init() {
   // clear variables
    memset(&spi_vars,0,sizeof(spi_vars_t));
   
    //enable SPI2 and GPIOB, Clock
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    //Configure SPI-related pins: PB.13 as SCLK pin ,PB.14 as MISO pin, PB.15 as MOSI pin, PB.12 as /SEL pin
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_13, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_13, LL_GPIO_PULL_DOWN);
    
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_14, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_14, LL_GPIO_PULL_DOWN);
    
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_15, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_15, LL_GPIO_PULL_DOWN);
    
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_12, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12);//set /SEL high
    //GPIOB->ODR |= 0X1000;
  
    
    //Configure SPI2 functional parameters 
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    LL_SPI_InitTypeDef spi_initstruct;
    //Configure SPI1 communication
    spi_initstruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV16; //div8 is also ok? yadong wan
    spi_initstruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    spi_initstruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
    spi_initstruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
    spi_initstruct.BitOrder          = LL_SPI_MSB_FIRST;
    spi_initstruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
    spi_initstruct.NSS               = LL_SPI_NSS_SOFT;
    spi_initstruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
    spi_initstruct.Mode              = LL_SPI_MODE_MASTER;

    LL_SPI_Init(SPI2, &spi_initstruct);
    /* Initialize FFIFO Threshold  1byte*/
    LL_SPI_SetRxFIFOThreshold(SPI2, LL_SPI_RX_FIFO_TH_QUARTER); 
    //enable SPI2 
    LL_SPI_Enable(SPI2);
    
   
#ifdef SPI_IN_INTERRUPT_MODE
    LL_SPI_EnableIT_RXNE(SPI2);//need for interrupt mode yadong wan??
    //Configure NVIC: Preemption Priority = 1 and Sub Priority = 1
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel	                = SPI2_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority	        = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd	                = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(SPI2_IRQn, NVIC_EncodePriority(5, 1, 2));
    NVIC_EnableIRQ(SPI2_IRQn);
#endif
}

#ifdef SPI_IN_INTERRUPT_MODE
void spi_setCallback(spi_cbt cb) {
   spi_vars.callback = cb;
}
#endif

void spi_txrx(uint8_t*     bufTx,
              uint8_t      lenbufTx,
              spi_return_t returnType,
              uint8_t*     bufRx,
              uint8_t      maxLenBufRx,
              spi_first_t  isFirst,
              spi_last_t   isLast) {

#ifdef SPI_IN_INTERRUPT_MODE
   // disable interrupts
   LL_SPI_DisableIT_RXNE(SPI2);             
   //NVIC_RESETPRIMASK();?? why disable all interrupts?
#endif
   
   // register spi frame to send
   spi_vars.pNextTxByte      =  bufTx;
   spi_vars.numTxedBytes     =  0;
   spi_vars.txBytesLeft      =  lenbufTx;
   spi_vars.returnType       =  returnType;
   spi_vars.pNextRxByte      =  bufRx;
   spi_vars.maxRxBytes       =  maxLenBufRx;
   spi_vars.isFirst          =  isFirst;
   spi_vars.isLast           =  isLast;
   
   // SPI is now busy
   spi_vars.busy             =  1;
   
   
   // lower CS signal to have slave listening
   if (spi_vars.isFirst==SPI_FIRST) {
     LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12);
     //GPIO_ResetBits(GPIOB, GPIO_Pin_12);
   }
   
#ifdef SPI_IN_INTERRUPT_MODE
   // implementation 1. use a callback function when transaction finishes
   
   // write first byte to TX buffer
   LL_SPI_TransmitData8(SPI2,*spi_vars.pNextTxByte);
  // SPI_I2S_SendData(SPI2,*spi_vars.pNextTxByte);
   // re-enable interrupts
   LL_SPI_EableIT_RXNE(SPI2);
   //NVIC_SETPRIMASK();
#else
   // implementation 2. busy wait for each byte to be sent
   // send all bytes
   while (spi_vars.txBytesLeft>0) {
      // write next byte to TX buffer
   LL_SPI_TransmitData8(SPI2,*spi_vars.pNextTxByte);

      // busy wait on the interrupt flag
      while (LL_SPI_IsActiveFlag_RXNE(SPI2) == RESET);
      
      // clear the interrupt flag
     // SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_RXNE);
      // save the byte just received in the RX buffer
      switch (spi_vars.returnType) {
         case SPI_FIRSTBYTE:
            if (spi_vars.numTxedBytes==0) {
               *spi_vars.pNextRxByte   = LL_SPI_ReceiveData8(SPI2);
            }
            break;
         case SPI_BUFFER:
            *spi_vars.pNextRxByte      = LL_SPI_ReceiveData8(SPI2);
            spi_vars.pNextRxByte++;
            break;
         case SPI_LASTBYTE:
            *spi_vars.pNextRxByte      = LL_SPI_ReceiveData8(SPI2);
            break;
      }
      // one byte less to go
      spi_vars.pNextTxByte++;
      spi_vars.numTxedBytes++;
      spi_vars.txBytesLeft--;
   }
   
   // put CS signal high to signal end of transmission to slave
   if (spi_vars.isLast==SPI_LAST) {
      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12);//set /SEL high
   }
   
   // SPI is not busy anymore
   spi_vars.busy             =  0;
#endif
}

//=========================== private =========================================

//=========================== interrupt handlers ==============================

kick_scheduler_t spi_isr() {
#ifdef SPI_IN_INTERRUPT_MODE
   // save the byte just received in the RX buffer
   switch (spi_vars.returnType) {
      case SPI_FIRSTBYTE:
         if (spi_vars.numTxedBytes==0) {
            *spi_vars.pNextRxByte = SPI_I2S_ReceiveData(SPI2);
         }
         break;
      case SPI_BUFFER:
         *spi_vars.pNextRxByte    = SPI_I2S_ReceiveData(SPI2);
         spi_vars.pNextRxByte++;
         break;
      case SPI_LASTBYTE:
         *spi_vars.pNextRxByte    = SPI_I2S_ReceiveData(SPI2);
         break;
   }
   
   // one byte less to go
   spi_vars.pNextTxByte++;
   spi_vars.numTxedBytes++;
   spi_vars.txBytesLeft--;
   
   if (spi_vars.txBytesLeft>0) {
      // write next byte to TX buffer
      SPI_SendData(SPI2,*spi_vars.pNextTxByte);
   } 
   else {
      // put CS signal high to signal end of transmission to slave
      if (spi_vars.isLast==SPI_LAST) {
          LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12);//set /SEL high
      }
      // SPI is not busy anymore
      spi_vars.busy          =  0;
      
      // SPI is done!
      if (spi_vars.callback!=NULL) {
         // call the callback
         spi_vars.callback();
         // kick the OS
         return 1;
      }
   }
#else
   while(1);// this should never happen
#endif
}
