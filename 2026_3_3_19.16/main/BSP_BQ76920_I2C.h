#ifndef _BSP_BQ76920_I2C_H__
#define _BSP_BQ76920_I2C_H__
#include "hal_data.h"
#include "BQ76920_REG.h"

#define BQ76920_I2C_TIMEOUT_MS      100

void BSP_BQ76920_Init(void);

void BSP_I2C_BQ76920_ByteWrite(unsigned char reg_addr,unsigned char data);

void BSP_BQ76920_BufferRead(uint8_t reg_addr, uint8_t *data);

fsp_err_t bq76920_read_registers(uint8_t reg_addr, uint8_t *data, uint8_t len);

static bool BQ769X0_ReadBlockWithCRC(uint8_t Register, uint8_t *buffer, uint8_t length);

static bool BQ769X0_WriteRegisterWordWithCRC(uint8_t Register, uint16_t data);

static bool BQ769X0_WriteRegisterByteWithCRC(uint8_t Register, uint8_t data);

static uint8_t CRC8(uint8_t *ptr, uint8_t len, uint8_t key);

static bool BQ769X0_ReadRegisterWordWithCRC(uint8_t Register, uint16_t *data);

static bool BQ769X0_ReadRegisterByteWithCRC(uint8_t Register, uint8_t *data);

static bool BQ769X0_WriteBlockWithCRC(uint8_t startAddress, uint8_t *buffer, uint8_t length);
#endif