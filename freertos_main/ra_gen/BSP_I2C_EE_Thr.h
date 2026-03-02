#ifndef BSP_I2C_EE_THR_H_
#define BSP_I2C_EE_THR_H_

#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#include "r_sci_i2c.h"
#include "r_i2c_master_api.h"
#include "BSP_IIC_EEPROM.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_I2C_EE_Thr_entry(void * pvParameters);

extern const i2c_master_cfg_t BSP_EEPROM_SCI_IIC_cfg;
extern const i2c_master_instance_t BSP_EEPROM_SCI_IIC;
void bsp_sci_i2c_master_callback(i2c_master_callback_args_t * p_args);

extern const sci_i2c_extended_cfg_t BSP_EEPROM_SCI_IIC_cfg_extend;
extern sci_i2c_instance_ctrl_t BSP_EEPROM_SCI_IIC_ctrl;

#ifdef __cplusplus
}
#endif

#endif /* BSP_I2C_EE_THR_H_ */