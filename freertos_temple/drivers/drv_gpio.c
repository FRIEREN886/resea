#include "drv_gpio.h"
#include <stdio.h>

static int IODrvInit(struct IODev *ptdev);
static int IODrvWrite(struct IODev *ptdev, IODevState_t level);
static IODevState_t IODrvRead(struct IODev *ptdev);


static IODev gLedDev = {
    .name = "UserLed",
    .Init = IODrvInit,
    .Write = IODrvWrite,
    .Read = IODrvRead
};

struct IODev *IOGetDecvice(const char *name)
{
    if(strcmp(name, "UserLed") == 0)
        return &gLedDev;

    return NULL;
}

static int IODrvInit(struct IODev *ptdev)
{
    if(ptdev->name == NULL)     return -1;
    
    /* 初始化LED灯GPIO */
    if(strcmp(ptdev->name, "UserLed") == 0)
    {
#if 0 
		/* 这个函数会初始化话所有pin, 不应该这样做, 会影响到正在使用的设备比如SPI */		
        fsp_err_t err = g_ioport.p_api->open(g_ioport.p_ctrl, g_ioport.p_cfg);
        if(FSP_ERR_ALREADY_OPEN == err)
        {
            printf("Error. GPIOs are already open and init.\r\n");
            return -1;
        }
        else if(FSP_SUCCESS == err)
            printf("Success to open Led device: %s!\r\n", ptdev->name);
#endif		
    }
    return 0;
}

static int IODrvWrite(struct IODev *ptdev, IODevState_t level)
{
    if(ptdev->name == NULL)     return -1;
    if(strcmp(ptdev->name, "UserLed") == 0)
    {
        fsp_err_t err = g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_IO_PORT_04_PIN_00, (bsp_io_level_t)level);
        if(FSP_SUCCESS != err)
        {
            printf("Error. Failed to write level in IO device: %s\r\n", ptdev->name);
            return -1;
        }
    }

    return 0;
}

static IODevState_t IODrvRead(struct IODev *ptdev)
{
    if(ptdev->name == NULL)    return ErrLevel;
    
    IODevState_t state = ErrLevel;
    if(strcmp(ptdev->name, "UserLed") == 0)
    {
        fsp_err_t err = g_ioport.p_api->pinRead(g_ioport.p_ctrl, BSP_IO_PORT_04_PIN_00, (bsp_io_level_t*)&state);
        if(FSP_SUCCESS != err)
        {
            printf("Error. Failed to write level in IO device: %s\r\n", ptdev->name);
            return ErrLevel;
        }
    }
    
    return state;
}
