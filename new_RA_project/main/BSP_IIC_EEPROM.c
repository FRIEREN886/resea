#include "BSP_IIC_EEPROM.h"

unsigned int TimeOut = 500;
i2c_master_event_t BSP_I2C_EEPROM_TX_FLAG_Callback_Event;//EEPROM I2C发送完成事件返回保存参数

void BSP_I2C_EEPROM_Init()//初始化
{
    R_SCI_I2C_Open(&BSP_EEPROM_SCI_IIC_ctrl,&BSP_EEPROM_SCI_IIC_cfg);
    R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_MILLISECONDS);
}

//回调函数
void bsp_sci_i2c_master_callback(i2c_master_callback_args_t *args)
{
    BSP_I2C_EEPROM_TX_FLAG_Callback_Event = args->event;
}


//单字节传输数据给EEPROM

void BSP_I2C_EEPROM_ByteWrite(unsigned char addr,unsigned char Byte)
{
    unsigned char Send_Buffer[2] = {};
    Send_Buffer[0] = addr;
    Send_Buffer[1] = Byte;
    R_SCI_I2C_Write(&BSP_EEPROM_SCI_IIC_ctrl,&Send_Buffer[0],2,false);
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_EEPROM_TX_FLAG_Callback_Event && TimeOut)
    {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        TimeOut--;
    }
    TimeOut = 250;
}

void BSP_I2C_WaitState()
{
    while(TimeOut)
    {
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        TimeOut--;
    }
    TimeOut = 250;
}

//页写入EEPROM
void BSP_I2C_EEPROM_PageWrite(unsigned char *ptr_write,unsigned char WriteAddr,uint16_t len)
{
    unsigned char Send_Buffer[9] = {};
    Send_Buffer[0] = WriteAddr;
    for(unsigned char i = 0;i<len;i++)
        {
            Send_Buffer[i+1] = *(ptr_write+i);
        }
     R_SCI_I2C_Write(&BSP_EEPROM_SCI_IIC_ctrl,&Send_Buffer[0],len+1,false);
     while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_EEPROM_TX_FLAG_Callback_Event && TimeOut)
        {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        TimeOut--;
        }
    TimeOut = 250;
    R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MILLISECONDS);
}
//将缓冲区的数据写入EEPROM
void BSP_I2C_EEPROM_BufferWrite(uint8_t *p_buffer,uint8_t WriteAddr,uint16_t len)
{
    uint16_t Page = 0,losePage = 0;
    uint16_t Addr = 0,count = 0;
    Addr = WriteAddr % EEPROM_PAGESIZE;
    count = EEPROM_PAGESIZE - Addr;
    Page = len / EEPROM_PAGESIZE;
    losePage = len % EEPROM_PAGESIZE;
    if(Addr == 0)
    {
        if(Page == 0)
            {
            BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,losePage);
            }
        else
            {
            while(Page--)
                {
                 BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,EEPROM_PAGESIZE);
                 WriteAddr += EEPROM_PAGESIZE;
                 p_buffer += EEPROM_PAGESIZE;
                }
            if(losePage != 0)
                {
                BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,losePage);
                }
            }
    }
    else
        {
        if(Page == 0)
            {
            BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,len);
            }
        else
            {
            len -= count;
            Page = len / EEPROM_PAGESIZE;
            losePage = len % EEPROM_PAGESIZE; 
            if(count != 0)
                {
                BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,count);
                WriteAddr += count;
                p_buffer += count;
                }
            while(Page--)
                {
                 BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,EEPROM_PAGESIZE);
                 WriteAddr += EEPROM_PAGESIZE;
                 p_buffer += EEPROM_PAGESIZE;
                }
           if(losePage != 0)
                {
                BSP_I2C_EEPROM_PageWrite(p_buffer,WriteAddr,losePage);
                }
            }
        }
}

//读取EEPROM数据
void BSP_EEPROM_BufferRead(unsigned char * ptr_buffer,unsigned char address,uint16_t byte)
{
    unsigned char Send_Buffer[2] = {};
    Send_Buffer[0] = address;
    R_SCI_I2C_Write(&BSP_EEPROM_SCI_IIC_ctrl,&Send_Buffer[0],1,true);
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_EEPROM_TX_FLAG_Callback_Event && TimeOut)
        {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
         TimeOut--;
        }
    TimeOut = 250;
    R_BSP_SoftwareDelay(50,BSP_DELAY_UNITS_MILLISECONDS);
    R_SCI_I2C_Read(&BSP_EEPROM_SCI_IIC_ctrl,ptr_buffer,byte,false);
    R_BSP_SoftwareDelay(50,BSP_DELAY_UNITS_MILLISECONDS);
}

uint8_t test()
{
    uint16_t  DATA = 20;
    int i = 0;
    unsigned char a[33] = {};
    unsigned char b[33] = {};
    printf("data:\n");
    for(i = 0;i < DATA;i++)
        {
            a[i] = (unsigned char)i;
            printf("0x%02x ",a[i]);                
        }
     printf("\n");
     BSP_I2C_EEPROM_BufferWrite(a,0x00,DATA);
     R_BSP_SoftwareDelay(50,BSP_DELAY_UNITS_MILLISECONDS);
     BSP_EEPROM_BufferRead(b,0x00,DATA);
     R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
     printf("Data:\n");
    for(i = 0;i < DATA;i++)
        {
            printf("0x%02x ",b[i]);      
        }
        printf("\n");
}