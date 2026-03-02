#include "BSP_I2C_EE_Thr.h"
#include <stdio.h>

void BSP_I2C_EE_Thr_entry(void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);
    

    while(1)
    {
        vTaskDelay(1);
    }
}