#include "FSP_UART.h"
#include "circular_queue.h"
#include "BSP_WIFI.h"

volatile bool Uart_Send_Seccuse_Ack = false;

void FSP_UART_Init()
{
    fsp_err_t err = FSP_SUCCESS;
    err = R_SCI_UART_Open(&UART_7_ctrl,&UART_7_cfg);
    assert(FSP_SUCCESS == err);
}


//void UART7_Int_Callback(uart_callback_args_t * p_args)
//{
//    switch(p_args -> event)
//    {
//        case UART_EVENT_RX_CHAR:
//        {
////            R_SCI_UART_Write(&UART_4_ctrl, (uint8_t *)(p_args -> data), 1);
////            break;
//            
//            Queue_Wirte(&Circular_queue,(uint8_t *)&(p_args->data),1);
//            
////             static uint8_t rx_temp; 
////            rx_temp = (uint8_t)(p_args->data); // 获取接收到的数据
////    
////            // 发送回去
////            R_SCI_UART_Write(&BSP_UART6_WIFI_ctrl, &rx_temp, 1);
//            break;
//        }
//        case UART_EVENT_TX_COMPLETE:
//        {
//            Uart_Send_Seccuse_Ack = true;
//            break;
//        }
//        default:
//            break;
//    }
//}


//#if defined __GNUC__ && !defined __clang__
//int _write(int fd,char *pBuffer,int size);
//int _write(int fd,char *pBuffer,int size)
//{
//    (void)fd;
//    R_SCI_UART_Write(&UART_7_ctrl,(uint8_t *)pBuffer,(uint32_t)size);
//    while(Uart_Send_Seccuse_Ack == false);
//    Uart_Send_Seccuse_Ack = false;
//    return size;
//}
//#else
//int fputc(int ch, FILE *f)
//{
//    (void)f;
//    if(TX_PRINTF_FLAG == 1)    
//        {
//        R_SCI_UART_Write(&UART_7_ctrl,(uint8_t *)&ch,1);
//        while(Uart_Send_Seccuse_Ack == false);
//        Uart_Send_Seccuse_Ack = false;
//        }
//    else if(TX_PRINTF_FLAG == 2)
//        {
//        R_SCI_UART_Write(&BSP_UART6_WIFI_ctrl,(uint8_t *)&ch,1); 
//        while(Uart6_Send_Seccuse_Ack == false);
//        Uart6_Send_Seccuse_Ack = false;    
//        }
//        
//    return ch;
//}
//#endif

/* 重定向 printf 输出 */
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //防止编译警告
int _write(int fd, char *pBuffer, int size)
{
    (void)fd;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
    pLogDevice->Write(pLogDevice, (unsigned char*)&ch, 1);
    return size;
}


int _read (int fd, char *pBuffer, int size)
{
    (void)fd;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
	/* 启动接收字符 */
    while(pLogDevice->Read(pLogDevice, (unsigned char*)&pBuffer, size) != size)
    {}
    return size;
}
#else
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
struct __FILE{
   int handle;
};
#endif
FILE __stdout;
int fputc(int ch, FILE *f)
{
    (void)f;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
    pLogDevice->Write(pLogDevice, (unsigned char*)&ch, 1);
    return ch;
}

/* 重写这个函数,重定向scanf */
int fgetc(FILE *f)
{
	uint8_t ch;
	
	(void)f;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
	/* 启动接收字符 */
    while(pLogDevice->Read(pLogDevice, (unsigned char*)&ch, 1) != 1)
    {}
	/* 回显 */
	{
		fputc(ch, &__stdout);
		
		/* 回车之后加换行 */
		if (ch == '\r')
		{
			fputc('\n', &__stdout);
		}
	}
    
	return (int)ch;
}
#endif

