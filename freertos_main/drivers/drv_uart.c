/*
 * drv_uart.c
 *
 *  Created on: 2023年3月30日
 *      Author: slhuan
 */

#include "drv_uart.h"
#include <stdio.h>

static volatile bool gUartTxCplt = false;

void UARTDrvInit(void)
{
    fsp_err_t err = g_uart7.p_api->open(g_uart7.p_ctrl, g_uart7.p_cfg);
    if(FSP_SUCCESS == err)
        printf("Success to open device: uart7\r\n");
    else
        printf("Failed to open device: uart7\r\n");
}

void sci_uart7_callback(uart_callback_args_t * p_args)
{
    if(UART_EVENT_TX_COMPLETE == p_args->event)
        gUartTxCplt = 1;
}

static void UARTWaitTxCplt(void)
{
    uint16_t wTimeout = 100;
    while(!gUartTxCplt && wTimeout)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        wTimeout--;
    }

    gUartTxCplt = false;
}

/* 重定向 printf 输出 */
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //防止编译警告
int _write(int fd, char *pBuffer, int size)
{
    (void)fd;
    
    g_uart7.p_api->write(g_uart7.p_ctrl, (uint8_t*)pBuffer, (uint32_t)size);
    UARTWaitTxCplt();
    return size;
}
#else
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
struct __FILE{
   int handle;
};
#endif
FILE __stdout;
int fputc(int ch, FILE *f)
{
    (void)f;
    
    g_uart7.p_api->write(g_uart7.p_ctrl, (uint8_t*)&ch, (uint32_t)1);
    UARTWaitTxCplt();

    return ch;
}
#endif
