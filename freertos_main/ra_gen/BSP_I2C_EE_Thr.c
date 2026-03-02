/* generated thread source file - do not edit */
#include "BSP_I2C_EE_Thr.h"

#if 0
                static StaticTask_t BSP_I2C_EE_Thr_memory;
                #if defined(__ARMCC_VERSION)           /* AC6 compiler */
                static uint8_t BSP_I2C_EE_Thr_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #else
                static uint8_t BSP_I2C_EE_Thr_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.BSP_I2C_EE_Thr") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #endif
                #endif
                TaskHandle_t BSP_I2C_EE_Thr;
                void BSP_I2C_EE_Thr_create(void);
                static void BSP_I2C_EE_Thr_func(void * pvParameters);
                void rtos_startup_err_callback(void * p_instance, void * p_data);
                void rtos_startup_common_init(void);
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
extern uint32_t g_fsp_common_thread_count;

                const rm_freertos_port_parameters_t BSP_I2C_EE_Thr_parameters =
                {
                    .p_context = (void *) NULL,
                };

                void BSP_I2C_EE_Thr_create (void)
                {
                    /* Increment count so we will know the number of threads created in the RA Configuration editor. */
                    g_fsp_common_thread_count++;

                    /* Initialize each kernel object. */
                    

                    #if 0
                    BSP_I2C_EE_Thr = xTaskCreateStatic(
                    #else
                    BaseType_t BSP_I2C_EE_Thr_create_err = xTaskCreate(
                    #endif
                        BSP_I2C_EE_Thr_func,
                        (const char *)"BSP_I2C_EE_Thr",
                        1024/4, // In words, not bytes
                        (void *) &BSP_I2C_EE_Thr_parameters, //pvParameters
                        1,
                        #if 0
                        (StackType_t *)&BSP_I2C_EE_Thr_stack,
                        (StaticTask_t *)&BSP_I2C_EE_Thr_memory
                        #else
                        & BSP_I2C_EE_Thr
                        #endif
                    );

                    #if 0
                    if (NULL == BSP_I2C_EE_Thr)
                    {
                        rtos_startup_err_callback(BSP_I2C_EE_Thr, 0);
                    }
                    #else
                    if (pdPASS != BSP_I2C_EE_Thr_create_err)
                    {
                        rtos_startup_err_callback(BSP_I2C_EE_Thr, 0);
                    }
                    #endif
                }
                static void BSP_I2C_EE_Thr_func (void * pvParameters)
                {
                    /* Initialize common components */
                    rtos_startup_common_init();

                    /* Initialize each module instance. */
                    

                    #if (1 == BSP_TZ_NONSECURE_BUILD) && (1 == 1)
                    /* When FreeRTOS is used in a non-secure TrustZone application, portALLOCATE_SECURE_CONTEXT must be called prior
                     * to calling any non-secure callable function in a thread. The parameter is unused in the FSP implementation.
                     * If no slots are available then configASSERT() will be called from vPortSVCHandler_C(). If this occurs, the
                     * application will need to either increase the value of the "Process Stack Slots" Property in the rm_tz_context
                     * module in the secure project or decrease the number of threads in the non-secure project that are allocating
                     * a secure context. Users can control which threads allocate a secure context via the Properties tab when
                     * selecting each thread. Note that the idle thread in FreeRTOS requires a secure context so the application
                     * will need at least 1 secure context even if no user threads make secure calls. */
                     portALLOCATE_SECURE_CONTEXT(0);
                    #endif

                    /* Enter user code for this thread. Pass task handle. */
                    BSP_I2C_EE_Thr_entry(pvParameters);
                }
