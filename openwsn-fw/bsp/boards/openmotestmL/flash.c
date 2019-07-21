/**
\brief openmoteSTM32 definition of the "leds" bsp module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#include "flash.h"
#include "stm32l4xx_hal_conf.h"
//#include "leds.h"
//#include "stdint.h"
//#include "string.h"

//=========================== defines =========================================

//=========================== prototypes ======================================

//=========================== public ==========================================


uint8_t flash_init(void)
{   
    /* Unlock the internal flash */
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
    return HAL_OK;
}
  
uint8_t flash_erasePage(uint32_t address)
{
  uint32_t PageError = 0;
  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;
  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
   /* Get the number of sector to erase from 1st sector*/
  eraseinitstruct.Banks = GetBank(address);
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.Page = GetPage(address);
  eraseinitstruct.NbPages = 1;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if (status != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
 }

uint8_t  flash_erase_optByte(void)
{
  return 1;
}

uint8_t flash_write(uint32_t address,uint64_t data)
{

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data) == HAL_OK)
  {
      return HAL_OK;
  }
  else
  {
      return HAL_ERROR;//error
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

}

uint8_t flash_write_optByte(uint32_t address,uint8_t data)
{
  uint8_t status;
  return status;
}

uint64_t flash_read(uint32_t address)
{
  uint64_t temp = 0x00;
  temp = *(uint64_t*)address;
  return temp;
}

uint16_t flash_read_optByte(uint32_t address)
{
  uint16_t temp = 0x00;
  return temp;
}

void flash_getID(uint32_t address)
{
  
}



/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;
  
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
  
  return page;
}


/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;
  
  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
  	/* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
  	/* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }
  
  return bank;
}

//=========================== private =========================================