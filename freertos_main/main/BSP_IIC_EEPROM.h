#ifndef _BSP_IIC_EEPROM_H__
#define _BSP_IIC_EEPROM_H__

#include "hal_data.h"
#include "stdio.h"
#include "BSP_I2C_EE_Thr.h"
#define EEPROM_PAGESIZE 8
void BSP_I2C_EEPROM_Init();//≥ı ºªØ
fsp_err_t BSP_EEPROM_BufferRead(uint8_t *ptr_buffer, uint8_t address, uint16_t byte);
fsp_err_t BSP_I2C_EEPROM_BufferWrite(uint8_t *p_buffer, uint8_t WriteAddr, uint16_t len);
fsp_err_t BSP_I2C_EEPROM_PageWrite(uint8_t *ptr_write, uint8_t WriteAddr, uint16_t len);
fsp_err_t BSP_I2C_EEPROM_ByteWrite(uint8_t addr, uint8_t data);
void test();

#endif