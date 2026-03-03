/* generated common source file - do not edit */
#include "common_data.h"
icu_instance_ctrl_t BQ769X0_ALERT_IRQ10_ctrl;
const external_irq_cfg_t BQ769X0_ALERT_IRQ10_cfg =
{
    .channel             = 10,
    .trigger             = EXTERNAL_IRQ_TRIG_RISING,
    .filter_enable       = false,
    .pclk_div            = EXTERNAL_IRQ_PCLK_DIV_BY_64,
    .p_callback          = BQ769X0_ALERT_IRQ10_Callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = &NULL,
#endif
    .p_extend            = NULL,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ10)
    .irq                 = VECTOR_NUMBER_ICU_IRQ10,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t BQ769X0_ALERT_IRQ10 =
{
    .p_ctrl        = &BQ769X0_ALERT_IRQ10_ctrl,
    .p_cfg         = &BQ769X0_ALERT_IRQ10_cfg,
    .p_api         = &g_external_irq_on_icu
};
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
        {
            .p_api = &g_ioport_on_ioport,
            .p_ctrl = &g_ioport_ctrl,
            .p_cfg = &g_bsp_pin_cfg,
        };
void g_common_init(void) {
}
