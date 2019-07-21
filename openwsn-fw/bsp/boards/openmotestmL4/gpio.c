/*author Yadong Wan  Feb. 2019
*/
#include "stm32l4xx_ll_lib.h"

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
  LL_GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIOD and GPIOE clock */
  
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOB
                           | LL_AHB2_GRP1_PERIPH_GPIOC |LL_AHB2_GRP1_PERIPH_GPIOD);
  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
//                         | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD 
//                         | RCC_APB2Periph_AFIO, ENABLE);
//  
  GPIO_InitStructure.Pin = LL_GPIO_PIN_ALL;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
  //GPIO_InitStructure.Pull = LL_GPIO_MODE_NO;
  //need pull down or pll up? Yadong Wan
  /* PA  */
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* PB  */
  LL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* PC  */
  LL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* PD  */
  LL_GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**
  * @brief  Configures the GPIO PD0.2 port.
  * @param  None
  * @retval : None
  */
void GPIO_Configuration(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOD clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
  /* Configure PD.02 as Output push-pull 50MHz */
  GPIO_InitStructure.Pin   = LL_GPIO_PIN_2;
  GPIO_InitStructure.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Mode       = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStructure.Pull       = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    /* Enable GPIOC clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  /* Configure PC.04 as Output push-pull 50MHz */
  GPIO_InitStructure.Pin        = LL_GPIO_PIN_4;
  GPIO_InitStructure.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Mode       = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStructure);

}