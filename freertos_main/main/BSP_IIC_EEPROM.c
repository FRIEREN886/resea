#include "BSP_I2C_EE_Thr.h"

#define EEPROM_PAGESIZE 8

SemaphoreHandle_t i2c_sem = NULL;
volatile i2c_master_event_t i2c_evt_flag = I2C_MASTER_EVENT_ABORTED;

void bsp_sci_i2c_master_callback(i2c_master_callback_args_t * args)
{
    i2c_evt_flag = args->event; 
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(i2c_sem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void BSP_I2C_EEPROM_Init(void)
{
    fsp_err_t err;

    i2c_sem = xSemaphoreCreateBinary();
    if (i2c_sem == NULL)
    {
        printf("Failed to create I2C semaphore!\n");
        while(1);
    }
    err = R_SCI_I2C_Open(&BSP_EEPROM_SCI_IIC_ctrl, &BSP_EEPROM_SCI_IIC_cfg);
    if (err != FSP_SUCCESS)
    {
        printf("I2C open failed! Error: %d\n", err);
        while(1);
    }
    vTaskDelay(10);
}

// µ¥×Ö½ÚÐ´
fsp_err_t BSP_I2C_EEPROM_ByteWrite(uint8_t addr, uint8_t data)
{
    uint8_t Send_Buffer[2] = {addr, data};
    i2c_evt_flag = I2C_MASTER_EVENT_ABORTED;

    fsp_err_t err = R_SCI_I2C_Write(&BSP_EEPROM_SCI_IIC_ctrl, Send_Buffer, 2, false);
    if (err != FSP_SUCCESS) return err;

    if(xSemaphoreTake(i2c_sem, pdMS_TO_TICKS(100)) != pdPASS)
    {
        printf("I2C ByteWrite Timeout!\n");
        return FSP_ERR_TIMEOUT;
    }
    if(i2c_evt_flag != I2C_MASTER_EVENT_TX_COMPLETE)
        return FSP_ERR_ABORTED;

    vTaskDelay(5); 
    return FSP_SUCCESS;
}

// Ò³Ð´
fsp_err_t BSP_I2C_EEPROM_PageWrite(uint8_t *ptr_write, uint8_t WriteAddr, uint16_t len)
{
    uint8_t Send_Buffer[EEPROM_PAGESIZE+1];
    if (len > EEPROM_PAGESIZE) return FSP_ERR_INVALID_ARGUMENT;
    Send_Buffer[0] = WriteAddr;
    for(uint16_t i=0; i<len; i++)
    {
        Send_Buffer[i+1] = ptr_write[i];
    }
    i2c_evt_flag = I2C_MASTER_EVENT_ABORTED;
    fsp_err_t err = R_SCI_I2C_Write(&BSP_EEPROM_SCI_IIC_ctrl, Send_Buffer, len+1, false);
    if (err != FSP_SUCCESS) return err;

    if(xSemaphoreTake(i2c_sem, pdMS_TO_TICKS(100)) != pdPASS)
    {
        printf("I2C PageWrite Timeout!\n");
        return FSP_ERR_TIMEOUT;
    }
    if(i2c_evt_flag != I2C_MASTER_EVENT_TX_COMPLETE)
        return FSP_ERR_ABORTED;

    vTaskDelay(10);
    return FSP_SUCCESS;
}

// »º³åÇøÐ´
fsp_err_t BSP_I2C_EEPROM_BufferWrite(uint8_t *p_buffer, uint8_t WriteAddr, uint16_t len)
{
    fsp_err_t err;
    while (len > 0)
    {
        uint8_t page_offset = WriteAddr % EEPROM_PAGESIZE;
        uint8_t page_remain = EEPROM_PAGESIZE - page_offset;
        uint8_t chunk = (len < page_remain) ? len : page_remain;

        err = BSP_I2C_EEPROM_PageWrite(p_buffer, WriteAddr, chunk);
        if (err != FSP_SUCCESS) return err;

        WriteAddr += chunk;
        p_buffer += chunk;
        len -= chunk;
    }
    return FSP_SUCCESS;
}

// ¶Á
fsp_err_t BSP_EEPROM_BufferRead(uint8_t *ptr_buffer, uint8_t address, uint16_t byte)
{
    uint8_t Send_Buffer[1] = {address};
    i2c_evt_flag = I2C_MASTER_EVENT_ABORTED;

    fsp_err_t err = R_SCI_I2C_Write(&BSP_EEPROM_SCI_IIC_ctrl, Send_Buffer, 1, true);
    if (err != FSP_SUCCESS) return err;

    if(xSemaphoreTake(i2c_sem, pdMS_TO_TICKS(100)) != pdPASS) {
        printf("I2C Addr Set Timeout!\n");
        return FSP_ERR_TIMEOUT;
    }

    i2c_evt_flag = I2C_MASTER_EVENT_ABORTED;
    err = R_SCI_I2C_Read(&BSP_EEPROM_SCI_IIC_ctrl, ptr_buffer, byte, false);
    if (err != FSP_SUCCESS) return err;

    if(xSemaphoreTake(i2c_sem, pdMS_TO_TICKS(100)) != pdPASS) {
        printf("I2C Data Read Timeout!\n");
        return FSP_ERR_TIMEOUT;
    }
    if(i2c_evt_flag != I2C_MASTER_EVENT_RX_COMPLETE)
        return FSP_ERR_ABORTED;

    return FSP_SUCCESS;
}

void test()
{
    BSP_I2C_EEPROM_Init();
    vTaskDelay(100);
    //uint8_t wbuf[16];
    uint8_t rbuf[16] = {0};
//    for(uint8_t i=0; i<sizeof(wbuf); i++) 
//        wbuf[i] = i + 1;
    uint8_t wbuf[16] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5};
    printf("Write: ");
    for(unsigned int i=0; i<sizeof(wbuf); i++) 
        printf("0x%02X ", wbuf[i]);
    printf("\n");
    
    if(BSP_I2C_EEPROM_BufferWrite(wbuf, 0x00, sizeof(wbuf)) != FSP_SUCCESS)
        printf("EEPROM Write Error!\n");
    vTaskDelay(50);

    if(BSP_EEPROM_BufferRead(rbuf, 0x00, sizeof(rbuf)) != FSP_SUCCESS)
        printf("EEPROM Read Error!\n");

    printf("Read : ");
    for(unsigned int  i=0; i < sizeof(rbuf); i++) printf("0x%02X ", rbuf[i]);
    printf("\n");
}