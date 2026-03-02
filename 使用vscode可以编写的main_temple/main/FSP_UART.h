#ifndef _FSP_UART_H__
#define _FSP_UART_H__

#include "hal_data.h"                   // Flex Software:Generated Data
#include "stdio.h"
#include "circular_queue.h"

extern uint8_t WIFI_Read_Buffer[DATA_LEN];
extern uint16_t WIFI_Read_Length;

void BSP_WIFI_Recive_Printf();
void FSP_UART_Init();

#endif