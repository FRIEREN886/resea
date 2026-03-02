/*
 * drv_gpio.h
 *
 *  Created on: 2023年4月13日
 *      Author: slhuan
 */

#ifndef DRV_GPIO_H_
#define DRV_GPIO_H_

#include "hal_data.h"

typedef enum{
    LowLevel,
    HighLevel,
    ErrLevel = -1
}IODevState_t;

typedef struct IODev{
    char         *name;
    int          (*Init)(struct IODev *ptDev);
    int          (*Write)(struct IODev *ptDev, IODevState_t level);
    IODevState_t (*Read)(struct IODev *ptDev);
}IODev, *PIODev;

struct IODev *IOGetDecvice(const char *name);

void KeyProcessJitter(uint32_t tick);

#endif /* DRV_GPIO_H_ */
