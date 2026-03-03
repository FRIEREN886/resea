#ifndef _BSP_IIC_EEPROM_H__
#define _BSP_IIC_EEPROM_H__

#include "hal_data.h"
#include "stdio.h"
#define EEPROM_PAGESIZE 8
void BSP_I2C_EEPROM_Init();//≥ı ºªØ
void BSP_EEPROM_BufferRead(unsigned char * ptr_buffer,unsigned char address,uint16_t byte);
void BSP_I2C_EEPROM_BufferWrite(uint8_t *p_buffer,uint8_t WriteAddr,uint16_t len);
void BSP_I2C_EEPROM_PageWrite(unsigned char *ptr_write,unsigned char WriteAddr,uint16_t len);
void BSP_I2C_EEPROM_ByteWrite(unsigned char addr,unsigned char Byte);

uint8_t test();
#endif