/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_sci_uart.h"
            #include "r_uart_api.h"
#include "r_sci_i2c.h"
#include "r_i2c_master_api.h"
FSP_HEADER
/** UART on SCI Instance. */
            extern const uart_instance_t      BSP_UART6_WIFI;

            /** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
            extern sci_uart_instance_ctrl_t     BSP_UART6_WIFI_ctrl;
            extern const uart_cfg_t BSP_UART6_WIFI_cfg;
            extern const sci_uart_extended_cfg_t BSP_UART6_WIFI_cfg_extend;

            #ifndef BSP_UART6_WIFI_Callback
            void BSP_UART6_WIFI_Callback(uart_callback_args_t * p_args);
            #endif
extern const i2c_master_cfg_t BSP_BQ76920_SCI_IIC_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t BSP_BQ76920_SCI_IIC;
#ifndef BSP_BQ76920_SCI_IIC_Callback
void BSP_BQ76920_SCI_IIC_Callback(i2c_master_callback_args_t * p_args);
#endif

extern const sci_i2c_extended_cfg_t BSP_BQ76920_SCI_IIC_cfg_extend;
extern sci_i2c_instance_ctrl_t BSP_BQ76920_SCI_IIC_ctrl;
extern const i2c_master_cfg_t BSP_EEPROM_SCI_IIC_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t BSP_EEPROM_SCI_IIC;
#ifndef bsp_sci_i2c_master_callback
void bsp_sci_i2c_master_callback(i2c_master_callback_args_t * p_args);
#endif

extern const sci_i2c_extended_cfg_t BSP_EEPROM_SCI_IIC_cfg_extend;
extern sci_i2c_instance_ctrl_t BSP_EEPROM_SCI_IIC_ctrl;
/** UART on SCI Instance. */
            extern const uart_instance_t      UART_7;

            /** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
            extern sci_uart_instance_ctrl_t     UART_7_ctrl;
            extern const uart_cfg_t UART_7_cfg;
            extern const sci_uart_extended_cfg_t UART_7_cfg_extend;

            #ifndef UART7_Int_Callback
            void UART7_Int_Callback(uart_callback_args_t * p_args);
            #endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
