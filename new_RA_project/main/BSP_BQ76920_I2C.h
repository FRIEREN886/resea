#ifndef _BSP_BQ76920_I2C_H__
#define _BSP_BQ76920_I2C_H__
#include "hal_data.h"
#include "BQ76920_REG.h"

#define BQ76920_I2C_TIMEOUT_MS      100

void BSP_BQ76920_Init(void);

void BSP_I2C_BQ76920_ByteWrite(unsigned char reg_addr,unsigned char data);

void BSP_BQ76920_BufferRead(uint8_t reg_addr, uint8_t *data);

fsp_err_t bq76920_read_registers(uint8_t reg_addr, uint8_t *data, uint8_t len);

#endif