#include "led_thread.h"
#include "drv_gpio.h"
#include "hal_data.h"
/* LedThread entry function */
/* pvParameters contains TaskHandle_t */
void led_thread_entry(void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    /* TODO: add your own code here */
    struct IODev *pLedDev = IOGetDecvice("UserLed");
    if(NULL != pLedDev)
        pLedDev->Init(pLedDev);
    bool state = false;
    while(1)
    {
        pLedDev->Write(pLedDev, state);
        state = !state;
        vTaskDelay(1000);
    }
}
