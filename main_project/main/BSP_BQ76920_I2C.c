#include "BSP_BQ76920_I2C.h"

i2c_master_event_t BSP_I2C_BQ76920_TX_FLAG_Callback_Event;
unsigned int BQ_TimeOut = 250;

void BSP_BQ76920_Init(void)
{
    R_SCI_I2C_Open(&BSP_BQ76920_SCI_IIC_ctrl,&BSP_BQ76920_SCI_IIC_cfg);
    R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_MILLISECONDS);
}

void BSP_BQ76920_SCI_IIC_Callback(i2c_master_callback_args_t *args)
{
    switch (args->event)
    {
        case I2C_MASTER_EVENT_TX_COMPLETE:
            BSP_I2C_BQ76920_TX_FLAG_Callback_Event = args->event;
            break;
            
        case I2C_MASTER_EVENT_RX_COMPLETE:
            BSP_I2C_BQ76920_TX_FLAG_Callback_Event = args->event;
            break;
            
        case I2C_MASTER_EVENT_ABORTED:
            BSP_I2C_BQ76920_TX_FLAG_Callback_Event = args->event;
            break;
            
    }
}

//单字节传输数据给BQ

void BSP_I2C_BQ76920_ByteWrite(unsigned char reg_addr,unsigned char data)
{
    unsigned char Send_Buffer[2] = {};
    Send_Buffer[0] = reg_addr;
    Send_Buffer[1] = data;
    R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl,&Send_Buffer[0],2,false);
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    BQ_TimeOut = 250;
}

//读取BQ数据

void BSP_BQ76920_BufferRead(uint8_t reg_addr, uint8_t *data)
{
    unsigned char Send_Buffer[2] = {};
    Send_Buffer[0] = reg_addr;
    R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl,&Send_Buffer[0],1,true);
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
        {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
         BQ_TimeOut--;
        }
    BQ_TimeOut = 250;
    R_BSP_SoftwareDelay(50,BSP_DELAY_UNITS_MILLISECONDS);
    R_SCI_I2C_Read(&BSP_BQ76920_SCI_IIC_ctrl,data,1,false);
    while(I2C_MASTER_EVENT_RX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
        {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
         BQ_TimeOut--;
        }
    BQ_TimeOut = 250;
}

//BQ批量读数据
fsp_err_t bq76920_read_registers(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    fsp_err_t err;
    
    unsigned char Send_Buffer[2] = {};
    Send_Buffer[0] = reg_addr;
    err = R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl,&Send_Buffer[0],1,true);
    if (FSP_SUCCESS != err)
    {
        return err;
    }
    
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
        {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
         BQ_TimeOut--;
        }
    BQ_TimeOut = 250;
    
    
    err = R_SCI_I2C_Read(&BSP_BQ76920_SCI_IIC_ctrl,data,len,false);
    if (FSP_SUCCESS != err)
    {
        return err;
    }
    
    while(I2C_MASTER_EVENT_RX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
        {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
         BQ_TimeOut--;
        }
    BQ_TimeOut = 250;
        
    return 0;
}
