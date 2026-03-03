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

// CRC8校验
static uint8_t CRC8(uint8_t *ptr, uint8_t len, uint8_t key)
{
	uint8_t i, crc=0;
	
	while (len-- != 0)
	{
		for (i = 0x80; i != 0; i /= 2)
		{
			if ((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= key;
			}
			else
			{
				crc *= 2;
			}

			if ((*ptr & i) != 0)
			{
				crc ^= key;
			}
		}
		ptr++;
	}
	return(crc);
}

static bool BQ769X0_WriteRegisterByteWithCRC(uint8_t Register, uint8_t data)
{
    uint8_t dataBuffer[4];
    
    dataBuffer[0] = BQ76920_I2C_ADDR << 1;
	dataBuffer[1] = Register;
	dataBuffer[2] = data;	
	dataBuffer[3] = CRC8(dataBuffer, 3, CRC_KEY);
    R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl,&dataBuffer[0],4,false);
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    BQ_TimeOut = 250;
}

static bool BQ769X0_WriteRegisterWordWithCRC(uint8_t Register, uint16_t data)
{
    uint8_t dataBuffer[6];

	dataBuffer[0] = BQ76920_I2C_ADDR << 1;
	dataBuffer[1] = Register;
	dataBuffer[2] = LOW_BYTE(data);	
	dataBuffer[3] = CRC8(dataBuffer, 3, CRC_KEY);
	dataBuffer[4] = HIGH_BYTE(data);	
	dataBuffer[5] = CRC8(dataBuffer + 4, 1, CRC_KEY);		
    R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl,&dataBuffer[0],6,false);
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
         R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    BQ_TimeOut = 250;
    return true;
}

static bool BQ769X0_ReadBlockWithCRC(uint8_t Register, uint8_t *buffer, uint8_t length)
{  	
    fsp_err_t err;
    uint8_t index, crcValue, crcInput[2];
    uint8_t *readData, *startData;

    // 分配内存用于存储读取的数据（每个字节后跟一个CRC字节）
    startData = (uint8_t *)BQ769X0_MALLOC(2 * length);
    if (NULL == startData)
    {
        BQ769X0_WARNING("Malloc Fail"); 
        return false;
    }
    readData = startData;

    // 第一步：写寄存器地址
    err = R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl, &Register, 1, true);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_FREE(startData);
        BQ769X0_ERROR("Write Register Address Fail");
        return false;
    }
    
    // 等待写完成
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_FREE(startData);
        BQ769X0_ERROR("I2C Write Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    // 第二步：读取数据（包含CRC字节）
    err = R_SCI_I2C_Read(&BSP_BQ76920_SCI_IIC_ctrl, readData, length * 2, false);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_FREE(startData);
        BQ769X0_ERROR("Read Register Block Fail");
        return false;
    }
    
    // 等待读完成
    while(I2C_MASTER_EVENT_RX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_FREE(startData);
        BQ769X0_ERROR("I2C Read Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    // CRC 校验 - 第一个字节
    crcInput[0] = (BQ76920_I2C_ADDR << 1) + 1;  // I2C读地址
    crcInput[1] = readData[0];

    crcValue = CRC8(crcInput, 2, CRC_KEY);
    readData++;
    
    if (crcValue != *readData)
    {
        BQ769X0_FREE(startData);
        BQ769X0_ERROR("Read Register Block CRC 1 Check Fail");
        return false;	
    }
    else
    {
        *buffer = *(readData - 1);
    }

    // CRC 校验 - 剩余字节
    for(index = 1; index < length; index++)
    {
        readData++;
        crcValue = CRC8(readData, 1, CRC_KEY);
        readData++;
        buffer++;

        if (crcValue != *readData)
        {
            BQ769X0_FREE(startData);
            BQ769X0_ERROR("Read Register Block CRC Check Fail");
            return false;		
        }
        else
        {
            *buffer = *(readData - 1);
        }
    }

    BQ769X0_FREE(startData);

    return true;
}

static bool BQ769X0_ReadRegisterWordWithCRC(uint8_t Register, uint16_t *data)
{  	
    fsp_err_t err;
    uint8_t readBuffer[4], crcInput[2], crcValue;

    // 第一步：写寄存器地址
    err = R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl, &Register, 1, true);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_ERROR("Write Register Address Fail");
        return false;
    }
    
    // 等待写完成
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_ERROR("I2C Write Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    // 第二步：读取4个字节（2个数据字节 + 2个CRC字节）
    err = R_SCI_I2C_Read(&BSP_BQ76920_SCI_IIC_ctrl, readBuffer, 4, false);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_ERROR("Read Register Word With CRC Fail");
        return false;
    }
    
    // 等待读完成
    while(I2C_MASTER_EVENT_RX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_ERROR("I2C Read Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    // CRC 校验 - 第一个字节
    crcInput[0] = (BQ76920_I2C_ADDR << 1) + 1;  // I2C读地址
    crcInput[1] = readBuffer[0];

    crcValue = CRC8(crcInput, 2, CRC_KEY);
    if (crcValue != readBuffer[1])
    {
        BQ769X0_ERROR("Read Register Word CRC 1 Check Fail");
        return false;
    }

    // CRC 校验 - 第二个字节
    crcValue = CRC8(readBuffer + 2, 1, CRC_KEY);
    if (crcValue != readBuffer[3])
    {
        BQ769X0_ERROR("Read Register Word CRC 2 Check Fail");
        return false;
    }

    // 组合成16位数据（低字节在前，高字节在后）
    *data = (readBuffer[2] << 8) | readBuffer[0];

    return true;
}

static bool BQ769X0_ReadRegisterByteWithCRC(uint8_t Register, uint8_t *data)
{  	
    fsp_err_t err;
    uint8_t readBuffer[2], crcInput[2], crcValue;

    // 第一步：写寄存器地址
    err = R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl, &Register, 1, true);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_ERROR("Write Register Address Fail");
        return false;
    }
    
    // 等待写完成
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_ERROR("I2C Write Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    // 第二步：读取2个字节（1个数据字节 + 1个CRC字节）
    err = R_SCI_I2C_Read(&BSP_BQ76920_SCI_IIC_ctrl, readBuffer, 2, false);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_ERROR("Read Register Byte With CRC Fail");
        return false;
    }
    
    // 等待读完成
    while(I2C_MASTER_EVENT_RX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_ERROR("I2C Read Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    // CRC 校验
    crcInput[0] = (BQ76920_I2C_ADDR << 1) + 1;  // I2C读地址
    crcInput[1] = readBuffer[0];

    crcValue = CRC8(crcInput, 2, CRC_KEY);
    if (crcValue != readBuffer[1])
    {
        BQ769X0_ERROR("Read Register Byte CRC Check Fail");
        return false;
    }

    // 返回读取的数据
    *data = readBuffer[0];

    return true;
}

static bool BQ769X0_WriteBlockWithCRC(uint8_t startAddress, uint8_t *buffer, uint8_t length)
{
    fsp_err_t err;
    uint8_t index;
    uint8_t *bufferCRC, *pointer;

    // 分配内存：寄存器地址(1) + I2C地址(1) + 数据和CRC(2*length)
    bufferCRC = (uint8_t *)BQ769X0_MALLOC(2 * length + 2);
    if (NULL == bufferCRC)
    {
        BQ769X0_WARNING("Malloc Fail");
        return false;
    }

    // 构建发送缓冲区
    pointer = bufferCRC;
    *pointer++ = BQ76920_I2C_ADDR << 1;  // I2C 写地址（用于CRC计算）
    *pointer++ = startAddress;            // 寄存器起始地址
    *pointer++ = *buffer;                 // 第一个数据字节
    *pointer = CRC8(bufferCRC, 3, CRC_KEY);  // 第一个字节的CRC

    // 填充剩余数据和CRC
    for(index = 1; index < length; index++)
    {
        pointer++;
        buffer++;
        *pointer = *buffer;                      // 数据字节
        *(pointer + 1) = CRC8(pointer, 1, CRC_KEY);  // 对应的CRC
        pointer++;
    }

    // 发送数据：跳过第一个字节（I2C地址），从寄存器地址开始
    // 发送内容：寄存器地址(1) + [数据(1) + CRC(1)] * length
    err = R_SCI_I2C_Write(&BSP_BQ76920_SCI_IIC_ctrl, bufferCRC + 1, 2 * length + 1, false);
    if (FSP_SUCCESS != err)
    {
        BQ769X0_FREE(bufferCRC);
        BQ769X0_ERROR("Write Register Block With CRC Fail");
        return false;
    }
    
    // 等待写完成
    while(I2C_MASTER_EVENT_TX_COMPLETE != BSP_I2C_BQ76920_TX_FLAG_Callback_Event && BQ_TimeOut)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        BQ_TimeOut--;
    }
    
    if (BQ_TimeOut == 0)
    {
        BQ769X0_FREE(bufferCRC);
        BQ769X0_ERROR("I2C Write Timeout");
        BQ_TimeOut = 250;  // 恢复超时计数器
        return false;
    }
    BQ_TimeOut = 250;  // 恢复超时计数器

    BQ769X0_FREE(bufferCRC);

    return true;	
}