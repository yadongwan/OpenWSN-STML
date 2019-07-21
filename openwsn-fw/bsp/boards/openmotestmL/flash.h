/**
\brief openmoteSTM32 definition of the FLASH.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/
#ifndef __FLASH_H
#define __FLASH_H

//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================
#include "stdint.h"

uint8_t     flash_init(void);
uint8_t  flash_erasePage(uint32_t address);
uint8_t  flash_erase_optByte(void);
uint8_t  flash_write(uint32_t address,uint64_t data);
uint8_t  flash_write_optByte(uint32_t address,uint8_t data);
uint64_t flash_read(uint32_t address);
uint16_t flash_read_optByte(uint32_t address);
void     flash_getID(uint32_t address);
static uint32_t GetBank(uint32_t Addr);
static uint32_t GetPage(uint32_t Addr);

#endif