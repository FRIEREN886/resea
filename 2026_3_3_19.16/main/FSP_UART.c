#include "FSP_UART.h"
#include "BSP_WIFI.h"

volatile bool Uart_Send_Seccuse_Ack = false;
uint8_t WIFI_Read_Buffer[DATA_LEN];
uint16_t WIFI_Read_Length;

void FSP_UART_Init()
{
    fsp_err_t err = FSP_SUCCESS;
    err = R_SCI_UART_Open(&UART_7_ctrl,&UART_7_cfg);
    assert(FSP_SUCCESS == err);
}


void UART7_Int_Callback(uart_callback_args_t * p_args)
{
    switch(p_args -> event)
    {
        case UART_EVENT_RX_CHAR:
        {
//            R_SCI_UART_Write(&UART_4_ctrl, (uint8_t *)(p_args -> data), 1);
//            break;
            
            Queue_Wirte(&Circular_queue,(uint8_t *)&(p_args->data),1);
            
//             static uint8_t rx_temp; 
//            rx_temp = (uint8_t)(p_args->data); // 获取接收到的数据
//    
//            // 发送回去
//            R_SCI_UART_Write(&BSP_UART6_WIFI_ctrl, &rx_temp, 1);
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Uart_Send_Seccuse_Ack = true;
            break;
        }
        default:
            break;
    }
}

void BSP_WIFI_Recive_Printf()
{
    TX_PRINTF_FLAG = 1;
        if(!Queue_isEmpty(&WIFI_Circular_queue))
        {
            uint16_t len = Queue_HadUse(&WIFI_Circular_queue);
            
            if(len > 0)
            {
                memset(WIFI_Read_Buffer, 0, sizeof(WIFI_Read_Buffer));
                Queue_Read(&WIFI_Circular_queue, WIFI_Read_Buffer, len);
                
                printf("Received: %s\r\n\r\n", WIFI_Read_Buffer);
            }
        }
        else
        {
            printf("No response!\r\n\r\n");
        }
}

#if defined __GNUC__ && !defined __clang__
int _write(int fd,char *pBuffer,int size);
int _write(int fd,char *pBuffer,int size)
{
    (void)fd;
    R_SCI_UART_Write(&UART_7_ctrl,(uint8_t *)pBuffer,(uint32_t)size);
    while(Uart_Send_Seccuse_Ack == false);
    Uart_Send_Seccuse_Ack = false;
    return size;
}
#else
int fputc(int ch, FILE *f)
{
    (void)f;
    if(TX_PRINTF_FLAG == 1)    
        {
        R_SCI_UART_Write(&UART_7_ctrl,(uint8_t *)&ch,1);
        while(Uart_Send_Seccuse_Ack == false);
        Uart_Send_Seccuse_Ack = false;
        }
    else if(TX_PRINTF_FLAG == 2)
        {
        R_SCI_UART_Write(&UART_7_ctrl,(uint8_t *)&ch,1);
        while(Uart_Send_Seccuse_Ack == false);
        Uart_Send_Seccuse_Ack = false;
        R_SCI_UART_Write(&BSP_UART6_WIFI_ctrl,(uint8_t *)&ch,1); 
        while(Uart6_Send_Seccuse_Ack == false);
        Uart6_Send_Seccuse_Ack = false;    
        }
        
    return ch;
}
#endif



