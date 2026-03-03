#ifndef _BSP_WIFI_H__
#define _BSP_WIFI_H__
#include "hal_data.h"
#include "circular_queue.h"

extern volatile bool Uart6_Send_Seccuse_Ack;
extern unsigned int TX_PRINTF_FLAG;



#define FSP_WIFI_AT printf("AT+\r\n"); //测试w800是否在线
#define FSP_WIFI_AT_Relod printf("AT+Z\r\n"); //软件复位w800
#define FSP_WIFI_AT_Show printf("AT+E\r\n"); //w800回显功能
#define FSP_WIFI_AT_ConnetMode printf("AT+WPRT=0\r\n"); //w800设置连接模式  0:STA;2:SoftAP;3:APSTA
#define FSP_WIFI_AT_Scan printf("AT+WSCAN\r\n"); //该指令仅在无线网络类型为STA时有效，用于扫描无线网络，完成后返回。
#define FSP_WIFI_AT_SetNetName printf("AT+SSID=USER304\r\n"); //w800设置wifi名字
#define FSP_WIFI_AT_SetNetKey printf("AT+KEY=1,0,liushenru6416\r\n"); //w800设置wifi密码 AT+KEY=[密钥格式],[密钥索引号],[密钥字符串]\r\n；密钥格式：0-Hex格式；1-ASCII格式；密钥索引号：1~4用于WEP加密密钥，其它加密方式固定为0；
#define FSP_WIFI_AT_Connet printf("AT+WJOIN\r\n"); //如果当前网络类型为为 STA 时，本指令功能为连接 AP。如果当前网络类型SoftAP或者APSTA 时，本指令功能为创建
#define FSP_WIFI_AT_LostConnet printf("AT+WLEAV\r\n"); //无线网络类型为 STA时，用于断开当前无线网络。
#define FSP_WIFI_AT_GetLocalNetInfo printf("AT+LKSTT\r\n"); //w800查询本端网络连接状态

typedef enum{
    TCP = 0,
    UDP = 1
}NetworkProtocol;

typedef enum{
    Client = 0,
    Server = 1
}LocalRole;

typedef struct{
    NetworkProtocol Protocl;
    LocalRole       Role;
    char            *IP;
    unsigned int    RemotePort;
    unsigned int    LocalPort;
    unsigned int    SocketPort;
}ConnectInfo;


void BSP_UART6_WIFI_Init(void);
void WIFI_Conet();

#endif