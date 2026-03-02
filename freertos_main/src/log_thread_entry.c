#include "log_thread.h"
#include "drv_uart.h"
#include "hal_data.h"
#include <stdio.h>
/* LogThread entry function */
/* pvParameters contains TaskHandle_t */
void log_thread_entry(void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    /* TODO: add your own code here */
    
    UARTDrvInit();
    
    uint32_t count = 0;
    while(1)
    {
        printf("\r\nLog: %d\r\n", count++);
        vTaskDelay(5000);
    }
}
