/**
\brief openmoteSTM32 definition of the "board" bsp module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, February 2012.
\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#include "board.h"
// bsp modules
#include "leds.h"
//#include "uart.h"
//#include "spi.h"
#include "bsp_timer.h"
//#include "radio.h"
//#include "radiotimer.h"
//#include "rcc.h"
//#include "nvic.h"
#include "debugpins.h"
#include "opentimers.h"
#include "gpio.h"

//=========================== variable ========================================
void SystemClock_Config(void);
//=========================== main ============================================

//extern int mote_main(void);

//int main() {
 //  return mote_main();
//}

//=========================== public ==========================================

void board_init()
{
  
   HAL_Init();
  
  /* Configure the system clock to 80 MHz */
   SystemClock_Config();
   leds_init();
   
   bsp_timer_init();
   

  
  
  
  /*
    RCC_Configuration();//Configure rcc
    NVIC_Configuration();//configure NVIC and Vector Table
    
    //configure ALL GPIO to AIN to get lowest power
    GPIO_Config_ALL_AIN();
    //configuration GPIO to measure the time from sleep to 72MHz
    GPIO_Configuration();
    
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    //enable GPIOB, Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    
    //Configure PB.01 as SLP_TR pin of RF
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    //Configure PB.11 as RST pin of RF
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //set /RST pin high(never reset)
    GPIO_SetBits(GPIOB, GPIO_Pin_11);
    
    // Configure PB.10 as input floating (EXTI Line10)
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIOB->ODR |= 0X0400;//set low
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);//Connect EXTI Line10 to PB.10
    EXTI_ClearITPendingBit(EXTI_Line10);

    //Configures EXTI line 10 to generate an interrupt on rising edge
    EXTI_InitTypeDef  EXTI_InitStructure; 
    EXTI_InitStructure.EXTI_Line    = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
    EXTI_Init(&EXTI_InitStructure);
    
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
   */
}

void board_sleep() {
   HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
  return; //ÏÈ²»Ë¯Ãß
/*  
#ifdef DEBUG_RUN_MODE
    // nothing need to do
#endif
  
#ifdef DEBUG_SLEEP_MODE
    DBGMCU_Config(DBGMCU_SLEEP, ENABLE);
    // Enable PWR and BKP clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    // Desable the SRAM and FLITF clock in sleep mode
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);
    // enter sleep mode
    __WFI();
#endif
  
#ifdef DEBUG_STOP_MODE
    uint16_t sleepTime = radiotimer_getPeriod() - radiotimer_getCapturedTime();
    
    DBGMCU_Config(DBGMCU_STOP, ENABLE);
    // Enable PWR and BKP clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    // Desable the SRAM and FLITF clock in Stop mode
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);
    // enter stop mode
    PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);
    
    if(sleepTime > 0)
    opentimers_sleepTimeCompesation(sleepTime);
#endif
  */
}



void board_reset(){
  ;
  //NVIC_GenerateSystemReset();
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


 

