#include "BSP_WIFI.h"
#include "circular_queue.h"
#include <string.h>
#include <stdio.h>
volatile bool Uart6_Send_Seccuse_Ack = false;
unsigned int  TX_PRINTF_FLAG = 1;


void BSP_UART6_WIFI_Init(void)
{
    fsp_err_t err = FSP_SUCCESS;
    err = R_SCI_UART_Open(&BSP_UART6_WIFI_ctrl,&BSP_UART6_WIFI_cfg);
    assert(FSP_SUCCESS == err);
}

//void BSP_UART6_WIFI_Callback(uart_callback_args_t * p_args)
//{
//    switch(p_args -> event)
//    {
//        case UART_EVENT_RX_CHAR:
//        {
//                //R_SCI_UART_Write(&UART_4_ctrl, (uint8_t *)(p_args -> data), 1);
//            
//              //Queue_Wirte(&WIFI_Circular_queue,(uint8_t *)&(p_args->data),1);
//            
//            static uint8_t rx_temp; 
//            rx_temp = (uint8_t)(p_args->data); // 获取接收到的数据
//    
//            // 发送回去
//            R_SCI_UART_Write(&UART_7_ctrl, &rx_temp, 1);
//            break;
//        }
//        case UART_EVENT_TX_COMPLETE:
//        {
//            Uart6_Send_Seccuse_Ack = true;
//            break;
//        }
//        default:
//            break;
//    }
//}

void BSP_UART6_WIFI_Callback(uart_callback_args_t * p_args)
{
    switch(p_args->event)
    {
        case UART_EVENT_RX_CHAR:
        {
            // 接收一个字符
            uint8_t rx_byte = (uint8_t)(p_args->data);
            Queue_Wirte(&WIFI_Circular_queue, &rx_byte, 1);
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Uart6_Send_Seccuse_Ack = true;
            break;
        }
        case UART_EVENT_ERR_OVERFLOW:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_PARITY:
        {
            // 错误处理
            break;
        }
        default:
            break;
    }
}

