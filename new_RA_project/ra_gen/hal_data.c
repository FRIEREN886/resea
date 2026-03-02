/* generated HAL source file - do not edit */
#include "hal_data.h"
sci_uart_instance_ctrl_t     BSP_UART6_WIFI_ctrl;

            baud_setting_t               BSP_UART6_WIFI_baud_setting =
            {
                /* Baud rate calculated with 0.469% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t BSP_UART6_WIFI_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &BSP_UART6_WIFI_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
            };

            /** UART interface configuration */
            const uart_cfg_t BSP_UART6_WIFI_cfg =
            {
                .channel             = 6,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = BSP_UART6_WIFI_Callback,
                .p_context           = NULL,
                .p_extend            = &BSP_UART6_WIFI_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (11),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI6_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI6_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI6_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI6_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI6_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t BSP_UART6_WIFI =
{
    .p_ctrl        = &BSP_UART6_WIFI_ctrl,
    .p_cfg         = &BSP_UART6_WIFI_cfg,
    .p_api         = &g_uart_on_sci
};
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t BSP_BQ76920_SCI_IIC_ctrl;
const sci_i2c_extended_cfg_t BSP_BQ76920_SCI_IIC_cfg_extend =
{
    /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */ .clock_settings.clk_divisor_value = 0, .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value = 30,
    .clock_settings.snfr_value         = (1),
};

const i2c_master_cfg_t BSP_BQ76920_SCI_IIC_cfg =
{
    .channel             = 5,
    .rate                = I2C_MASTER_RATE_STANDARD,
    .slave               = 0x18,
    .addr_mode           = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
    .p_transfer_tx       = NULL,
#else
    .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
    .p_transfer_rx       = NULL,
#else
    .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
    .p_callback          = BSP_BQ76920_SCI_IIC_Callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_SCI5_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI5_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI5_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI5_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (10),    /* (BSP_IRQ_DISABLED) is unused */
    .p_extend            = &BSP_BQ76920_SCI_IIC_cfg_extend,
};
/* Instance structure to use this module. */
const i2c_master_instance_t BSP_BQ76920_SCI_IIC =
{
    .p_ctrl        = &BSP_BQ76920_SCI_IIC_ctrl,
    .p_cfg         = &BSP_BQ76920_SCI_IIC_cfg,
    .p_api         = &g_i2c_master_on_sci
};
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t BSP_EEPROM_SCI_IIC_ctrl;
const sci_i2c_extended_cfg_t BSP_EEPROM_SCI_IIC_cfg_extend =
{
    /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */ .clock_settings.clk_divisor_value = 0, .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value = 30,
    .clock_settings.snfr_value         = (1),
};

const i2c_master_cfg_t BSP_EEPROM_SCI_IIC_cfg =
{
    .channel             = 4,
    .rate                = I2C_MASTER_RATE_STANDARD,
    .slave               = 0x50,
    .addr_mode           = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
    .p_transfer_tx       = NULL,
#else
    .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
    .p_transfer_rx       = NULL,
#else
    .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
    .p_callback          = bsp_sci_i2c_master_callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_SCI4_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI4_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI4_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI4_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (11),    /* (BSP_IRQ_DISABLED) is unused */
    .p_extend            = &BSP_EEPROM_SCI_IIC_cfg_extend,
};
/* Instance structure to use this module. */
const i2c_master_instance_t BSP_EEPROM_SCI_IIC =
{
    .p_ctrl        = &BSP_EEPROM_SCI_IIC_ctrl,
    .p_cfg         = &BSP_EEPROM_SCI_IIC_cfg,
    .p_api         = &g_i2c_master_on_sci
};
sci_uart_instance_ctrl_t     UART_7_ctrl;

            baud_setting_t               UART_7_baud_setting =
            {
                /* Baud rate calculated with 0.469% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t UART_7_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &UART_7_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
            };

            /** UART interface configuration */
            const uart_cfg_t UART_7_cfg =
            {
                .channel             = 7,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = UART7_Int_Callback,
                .p_context           = NULL,
                .p_extend            = &UART_7_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI7_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI7_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI7_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI7_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI7_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t UART_7 =
{
    .p_ctrl        = &UART_7_ctrl,
    .p_cfg         = &UART_7_cfg,
    .p_api         = &g_uart_on_sci
};
void g_hal_init(void) {
g_common_init();
}
