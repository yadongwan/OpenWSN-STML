#include "stm32l4xx_hal.h"
#include "gpio.h"


//=========================== defines =========================================

//=========================== variables =======================================

//=========================== public ===========================================
/**
  * @brief  Configures the different GPIO ports as Analog Inputs.
  * @param  None
  * @retval : None
  */
void GPIO_Config_ALL_AIN(void)
{
  
   static GPIO_InitTypeDef  GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
 
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);   
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);    
  
  GPIO_InitTypeDef GPIO_InitStructure;

  
}

/**
  * @brief  Configures the GPIO PD0.2 port.
  * @param  None
  * @retval : None
  */
void GPIO_Configuration(void)
{
  /*GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  // Configure PD.02 as Output push-pull 50MHz 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    // Enable GPIOC clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  // Configure PC.04 as Output push-pull 50MHz 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
*/
}