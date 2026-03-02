#include "BSP_WIFI.h"
#include "FSP_UART.h"
//#include <libs.h>
#include "hal_data.h"

/////////////////////////////
static int RingBufferWrite(struct RingBuffer *ptbuf, const unsigned char *src, unsigned int length);
static int RingBufferRead(struct RingBuffer *ptbuf, unsigned char *dst, unsigned int length);
static int RingBufferClear(struct RingBuffer *ptbuf);
static int RingBufferFree(struct RingBuffer *ptbuf);

/*
    函数名：RingBufferNew
    功能：初始化一个指定的环形缓冲区
    输入参数：ength->表示缓冲区分配的内存大小，单位是字节
    输出参数：无
    返回值：NULL->表示错误；ptbuf->表示成功得到一个buffer
*/
struct RingBuffer *RingBufferNew(unsigned int length)
{
    struct RingBuffer *ptbuf;
    if(0 == length)     return NULL;
    
    ptbuf = (struct RingBuffer*)malloc(sizeof(struct RingBuffer));
    if(NULL == ptbuf)   return NULL;
    if(NULL != ptbuf->info.pHead)
    {
        free(ptbuf->info.pHead);
    }
    ptbuf->info.pHead = (uint8_t*)malloc(length);
    if(NULL == ptbuf->info.pHead) 
    {
        printf("Error. Malloc %d bytes failed.\r\n", length);
        return false;
    }
    ptbuf->info.pValid = ptbuf->info.pValidEnd = ptbuf->info.pHead;
    ptbuf->info.pEnd = ptbuf->info.pHead + length;
    ptbuf->info.nValidLength = 0;
    ptbuf->info.nBufferLength = length;
    
    ptbuf->Write = RingBufferWrite;
    ptbuf->Read = RingBufferRead;
    ptbuf->Clear = RingBufferClear;
    ptbuf->Free = RingBufferFree;
    
    return ptbuf;
}

static int RingBufferFree(struct RingBuffer *ptbuf)
{
    if(ptbuf == NULL)           return -EINVAL;
    if(ptbuf->info.pHead==NULL) return -EINVAL;
    
    free((uint8_t*)ptbuf->info.pHead);
    
    ptbuf->info.pHead = NULL;
    ptbuf->info.pValid = NULL;
    ptbuf->info.pValidEnd = NULL;
    ptbuf->info.pEnd = NULL;
    ptbuf->info.nValidLength = 0;
    
    free((struct RingBuffer *)ptbuf);
    return ESUCCESS;
}

static int RingBufferWrite(struct RingBuffer *ptbuf, const unsigned char *src, unsigned int length)
{
    unsigned int len1 = 0, len2 = 0;
    unsigned int move_len = 0;
    
    if(length > ptbuf->info.nBufferLength)
    {
        return -EINVAL;
    }
    if(ptbuf->info.pHead==NULL)
    {
        return -EINVAL;
    }
    
    // copy buffer to pValidEnd
    if( (ptbuf->info.pValidEnd + length) > ptbuf->info.pEnd )  // 超过了Buffer范围需要分为两段
    {
        len1 = (unsigned)(ptbuf->info.pEnd - ptbuf->info.pValidEnd);
        len2 = length - len1;
        
        memcpy((uint8_t*)ptbuf->info.pValidEnd, src, len1);
        memcpy((uint8_t*)ptbuf->info.pHead, src + len1, len2);
        
        ptbuf->info.pValidEnd = ptbuf->info.pHead + len2;   // 更新有效数据区尾地址
    }
    else
    {
        memcpy((uint8_t*)ptbuf->info.pValidEnd, src, length);
        ptbuf->info.pValidEnd = ptbuf->info.pValidEnd + length;
    }
    
    // 重新计算已使用区的起始位置
    if( (ptbuf->info.nValidLength + length) > ptbuf->info.nBufferLength )     // 要写入的数据超过了缓冲区总长度，分为两段写
    {
        move_len = ptbuf->info.nValidLength + length - ptbuf->info.nBufferLength;
        if( (ptbuf->info.pValid + move_len) > ptbuf->info.pEnd )
        {
            len1 = (unsigned)(ptbuf->info.pEnd - ptbuf->info.pValid);
            len2 = move_len - len1;
            
            ptbuf->info.pValid = ptbuf->info.pHead + len2;
        }
        else
        {
            ptbuf->info.pValid = ptbuf->info.pValid + move_len;
        }
        
        ptbuf->info.nValidLength = ptbuf->info.nBufferLength;
    }
    else
    {
        ptbuf->info.nValidLength = ptbuf->info.nValidLength + length;
    }
    
    return (int)length;
}

static int RingBufferRead(struct RingBuffer *ptbuf, unsigned char *dst, unsigned int length)
{
    unsigned int len1 = 0, len2 = 0;
    if(ptbuf->info.pHead==NULL)     return -EINVAL;
    if(ptbuf->info.nValidLength==0) return -ENOMEM;
    
    if(length > ptbuf->info.nValidLength)
    {
        length = ptbuf->info.nValidLength;
    }
    
    if( (ptbuf->info.pValid + length) > ptbuf->info.pEnd )
    {
        len1 = (unsigned int)(ptbuf->info.pEnd - ptbuf->info.pValid);
        len2 = length - len1;
        
        memcpy(dst, (uint8_t*)ptbuf->info.pValid, len1);
        memcpy(dst + len1, (uint8_t*)ptbuf->info.pHead, len2);
        
        ptbuf->info.pValid = ptbuf->info.pHead + len2;
    }
    else
    {
        memcpy(dst, (uint8_t*)ptbuf->info.pValid, length);
        ptbuf->info.pValid = ptbuf->info.pValid + length;
    }
    
    ptbuf->info.nValidLength -= length;
    
    return (int)length;
}

static int RingBufferClear(struct RingBuffer *ptbuf)
{
    if(ptbuf == NULL)           return -EINVAL;
    if(ptbuf->info.pHead==NULL) return -EINVAL;
    if(ptbuf->info.pHead != NULL)
    {
        memset(ptbuf->info.pHead, 0, ptbuf->info.nBufferLength);
    }
    
    ptbuf->info.pValid = ptbuf->info.pValidEnd = ptbuf->info.pHead;
    ptbuf->info.nValidLength = 0;
    return ESUCCESS;
}

unsigned int TX_PRINTF_FLAG = 1;

static struct UartDev *gHeadUartDev;


static void UART6WaitTxCplt(void);
static void UART7WaitTxCplt(void);
static void UART7WaitRxCplt(void);

static int UARTDrvInit(struct UartDev *ptdev);
static int UARTDrvRead(struct UartDev *ptdev, unsigned char *buf, unsigned int length);
static int UARTDrvWrite(struct UartDev *ptdev, unsigned char * const buf, unsigned int length);

static volatile bool gUart6TxCplt = false;
static volatile bool gUart7TxCplt = false;
static volatile bool gUart7RxCplt = false;
static struct RingBuffer *gLogBuffer = NULL;
static struct RingBuffer *gWiFiBuffer = NULL;
static struct UartDev gWiFiDevice = {
    .name = "WiFi Uart",
    .channel = 6,
    .Init = UARTDrvInit,
    .Read = UARTDrvRead,
    .Write = UARTDrvWrite,
    .next = NULL
};

static struct UartDev gLogDevice = {
    .name = "Log Uart",
    .channel = 7,
    .Init = UARTDrvInit,
    .Read = UARTDrvRead,
    .Write = UARTDrvWrite,
    .next = NULL
};

void UartDevicesCreate(void)
{
    UartDeviceInsert(&gLogDevice);
    UartDeviceInsert(&gWiFiDevice);
    gLogDevice.Init(&gLogDevice);
    gLogDevice.Init(&gWiFiDevice);
}

static int UARTDrvInit(struct UartDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    switch(ptdev->channel)
    {
        case 0:case 1:case 2:
        case 3:case 4:case 5:
        case 6:
        {
            fsp_err_t err = BSP_UART6_WIFI.p_api->open(BSP_UART6_WIFI.p_ctrl, BSP_UART6_WIFI.p_cfg);
            assert(FSP_SUCCESS == err);
            gWiFiBuffer = RingBufferNew(1024);
            assert(NULL != gWiFiBuffer);
            break;
        }
        case 7:
        {
            fsp_err_t err = UART_7.p_api->open(UART_7.p_ctrl, UART_7.p_cfg);
            assert(FSP_SUCCESS == err);
            gLogBuffer = RingBufferNew(1024);
            assert(NULL != gLogBuffer);
            break;
        }
        case 8:case 9:
            break;
        default:break;
    }
    
    return ESUCCESS;
}

static int UARTDrvWrite(struct UartDev *ptdev, unsigned char * const buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;
    
    switch(ptdev->channel)
    {
        case 0:case 1:case 2:
        case 3:case 4:case 5:
        case 6:
        {
            fsp_err_t err = BSP_UART6_WIFI.p_api->write(BSP_UART6_WIFI.p_ctrl, buf, length);
            assert(FSP_SUCCESS == err);
            UART6WaitTxCplt();
            break;
        }
        case 7:
        {
            fsp_err_t err = UART_7.p_api->write(UART_7.p_ctrl, buf, length);
            assert(FSP_SUCCESS == err);
            UART7WaitTxCplt();
            break;
        }
        case 8:case 9:
            break;
        default:break;
    }
    return ESUCCESS;
}
static int UARTDrvRead(struct UartDev *ptdev, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;

    switch(ptdev->channel)
    {
        case 0:case 1:case 2:
        case 3:case 4:case 5:
        case 6:
        {
            if(gWiFiBuffer->Read(gWiFiBuffer, buf, length) != length)
                return -EIO;
            break;
        }
        case 7:
        {
            if(gLogBuffer->Read(gLogBuffer, buf, length) != length)
                return -EIO;
            break;
        }
        case 8:case 9:
            break;
        default:break;
    }
    
    return (int)length;
}

void BSP_UART6_WIFI_Callback(uart_callback_args_t * p_args)
{
    switch(p_args->event)
    {
        case UART_EVENT_RX_COMPLETE:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            gUart6TxCplt = true;
            break;
        }
        case UART_EVENT_RX_CHAR:
        {
            gWiFiBuffer->Write(gWiFiBuffer, (unsigned char*)&p_args->data, 1);
            break;
        }
        case UART_EVENT_ERR_PARITY:case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
            break;
        default:break;   
    }
}

void UART7_Int_Callback(uart_callback_args_t * p_args)
{
    switch(p_args->event)
    {
        case UART_EVENT_RX_COMPLETE:
        {
            gUart7RxCplt = true;
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            gUart7TxCplt = true;
            break;
        }
        case UART_EVENT_RX_CHAR:
        {
            gLogBuffer->Write(gLogBuffer, (unsigned char*)&p_args->data, 1);
            break;
        }
        case UART_EVENT_ERR_PARITY:case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
            break;
        default:break;   
    }
}

static void UART6WaitTxCplt(void)
{
    while(!gUart6TxCplt);
    gUart6TxCplt = false;
}

static void UART7WaitTxCplt(void)
{
    while(!gUart7TxCplt);
    gUart7TxCplt = false;
}

static void UART7WaitRxCplt(void)
{
    while(!gUart7RxCplt);
    gUart7RxCplt = false;
}


void UartDevicesRegister(void)
{
    UartDevicesCreate();
    UartDeviceList();
}

void UartDeviceInsert(struct UartDev *ptdev)
{
    if(NULL == gHeadUartDev)
        gHeadUartDev = ptdev;
    else
    {
        ptdev->next = gHeadUartDev;
        gHeadUartDev = ptdev;
    }
}

struct UartDev *UartDeviceFind(const char *name)
{
    struct UartDev *ptdev = gHeadUartDev;
    while(ptdev)
    {
        if(strstr(ptdev->name, name))
        {
            return ptdev;
        }
        ptdev = ptdev->next;
    }
    return NULL;
}

void UartDeviceList(void)
{
    struct UartDev *ptdev = gHeadUartDev;
    printf("\r\nUart Device List:\r\n");
    while(ptdev)
    {
        printf("\t%s\r\n", ptdev->name);
        ptdev = ptdev->next;
    }
    printf("\r\n");
}

/*-----------------------------------*/
static UartDevice *pWiFiBtDev = NULL;
static int WiFiBtDevCmdRet(const char *ret, unsigned int timeout);
static struct RingBuffer *gNetBuffer = NULL;

int WiFiBtDevInit(void)
{
    pWiFiBtDev = UartDeviceFind("WiFi");
    if(NULL==pWiFiBtDev) return -ENXIO;
    if(pWiFiBtDev->Init(pWiFiBtDev) != ESUCCESS)    return -EIO;
    
    int ret = -EIO;
    
    ret = WiFiBtDevReset();
    if(ESUCCESS != ret) return ret;
    R_BSP_SoftwareDelay(2000,BSP_DELAY_UNITS_MILLISECONDS);
    ret = WiFiBtDevSetWorkType(STA);
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevEnableDHCP();
    if(ESUCCESS != ret) return ret;
    gNetBuffer = RingBufferNew(1024);
    assert(NULL != gNetBuffer);
    
    return ret;
}

static unsigned char gDetectBuff[256] = {0};
static unsigned int gSocketPort = 0;
static unsigned int gDataLength = 0;
static unsigned char backcnt = 0;
void WiFiNetDataPreDeal(unsigned char data)
{
    static unsigned int index = 0;
    static unsigned int status = 0;
    unsigned char *pbuf = gDetectBuff;
    unsigned int i = index;
    pbuf[i] = data;
    index = (index + 1)%256;
    switch(status)
    {
        case 0: // Head Status
        {
            if(strstr((char*)pbuf, "+SKTRPT="))
            {
                status = 1;
                index = 0;
            }
            break;
        }
        case 1: // Socket Num Status;
        {
            if(pbuf[i] == ',')
            {
                for(unsigned int m=0; m<i; m++)
                {
                    gSocketPort = gSocketPort * 10 + pbuf[m] - '0';
                }
                status = 2;
                index = 0;
            }
            else if(i >= 9)
            {
                status = 0;
                index = 0;
            }
            break;
        }
        case 2: // Data Length Status
        {
            if(pbuf[i] == ',')
            {
                for(unsigned int m=0; m<i; m++)
                {
                    gDataLength = gDataLength * 10 + pbuf[m] - '0';
                }
                status = 3;
                index = 0;
            }
            else if(i >= 9)
            {
                status = 0;
                index = 0;
            }
            break;
        }
        case 3: // Find double \\r\\n
        {
            if(pbuf[i]=='\n')
            {
                backcnt++;
            }
            if(backcnt == 2)
            {
                status = 4;
                index = 0;
            }
            break;
        }
        case 4: // Net Data Save in RingBuffer
        {
            if(index == gDataLength)
            {
                if(NULL == gNetBuffer)  return;
                gNetBuffer->Write(gNetBuffer, pbuf, gDataLength);
                status = 0;
                backcnt = 0;
                index = 0;
                gDataLength = 0;
                gSocketPort = 0;
                memset(gDetectBuff, 0, 256);
            }
            break;
        }
        default:break;
    }
}

static int WiFiBtDevCmdRet(const char *ret, unsigned int timeout)
{
    unsigned char i = 0;
    unsigned char buf[256] = {0};
    while(timeout)
    {
        if(strstr((char*)buf, ret))
        {
            xprintf("%s\r\n", buf);
            return ESUCCESS;
        }
        else if(strstr((char*)buf, "+ERR"))
        {
            xprintf("%s\r\n", buf);
            return -EIO;
        }
        else if(pWiFiBtDev->Read(pWiFiBtDev, &buf[i], 1)==1)
        {
            i = (i+1)%256;
        }
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        timeout--;
    }
    xprintf("%s\r\n", buf);
    return -EIO;
}

int WiFiBtDevReset(void)
{
    char str[64];
    sprintf(str, "AT+Z\r\n");
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    return WiFiBtDevCmdRet("+OK", 10000);
}

int WiFiBtDevSetWorkType(WorkType type)
{
    char str[64];
    sprintf(str, "AT+WPRT=%d\r\n", type);
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    return WiFiBtDevCmdRet("+OK", 100);
}

int WiFiBtDevSetRPTM(void)
{
    char str[64];
    sprintf(str, "AT+SKRPTM=1\r\n");
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    return WiFiBtDevCmdRet("+OK", 100);
}

int WiFiBtDevEnableDHCP(void)
{
    int ret = -EIO;
    char *str = "AT+NIP=0\r\n";
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 100);
    return ret;
}

int WiFiBtDevDisableDHCP(const char *ip, const char *netmask, const char *gateway)
{
    int ret = -EIO;
    char str[64];
    sprintf(str, "AT+NIP=1,%s,%s,%s\r\n", ip, netmask, gateway);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 100);
    return ret;
}

int WiFiBtDevNetScan(void)
{
    const char *str="AT+WSCAN\r\n";
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    unsigned short timeout = 5000;
    while(timeout)
    {
        unsigned char c = 0;
        if(pWiFiBtDev->Read(pWiFiBtDev, &c, 1)==1)
        {
        }
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        timeout--;
    }
    return ESUCCESS;
}

int WiFiBtDevConnectWiFi(const char *name, const char *password)
{
    int ret = -EIO;

    char ssid[32];
    sprintf(ssid, "AT+SSID=%s\r\n", name);
    xprintf("%s\r\n", ssid);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)ssid, strlen(ssid));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
    if(ESUCCESS != ret) return ret;
    
    char key[32] ;
    sprintf(key, "AT+KEY=1,0,%s\r\n", password);
    xprintf("%s\r\n", key);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)key, strlen(key));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
    if(ESUCCESS != ret) return ret;
    
    char join[32] = "AT+WJOIN\r\n";
    xprintf("%s\r\n", join);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)join, strlen(join));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
    if(ret==ESUCCESS)   R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
    return ret;
}

int WiFiBtDevDisconnectWiFi(void)
{
    int ret = -EIO;
    char *leavw = "AT+WLEAV\r\n";
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)leavw, strlen(leavw));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OKret", 100);
    return ret;
}

int WiFiBtDevGetLocalIP(void)
{
    int ret = -EIO;
    char lkstt[32] = "AT+LKSTT\r\n";
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)lkstt, strlen(lkstt));
    if(ESUCCESS != ret) return ret;
    unsigned short timeout = 1000;
    while(timeout)
    {
        unsigned char c = 0;
        if(pWiFiBtDev->Read(pWiFiBtDev, &c, 1)==1)
        {
        }
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
        timeout--;
    }
    return ESUCCESS;
}

int WiFiBtDevConnect(ConnectInfo *info)
{
    int ret = -EIO;
    char skct[128];
    sprintf(skct, "AT+SKCT=%d,%d,%s,%d,%d\r\n", \
                   info->Protocl, \
                   info->Role, \
                   info->IP, \
                   info->RemotePort, \
                   info->LocalPort
                   );
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)skct, strlen(skct));
    if(ESUCCESS != ret) return ret;
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    /* 获取连接成功后的socket号 */
    unsigned short timeout = 100;
    unsigned char i = 0, status = 0;
    unsigned char buf[64] = {0};
    while(timeout!=0 && status != 2)
    {
        if(pWiFiBtDev->Read(pWiFiBtDev, &buf[i], 1)==1)
        {
            xprintf("%c", buf[i]);
            switch(status)
            {
                case 0:
                {
                    if(strstr((char*)buf, "+OK="))
                    {
                        status = 1;
                    }
                    if(strstr((char*)buf, "+ERR="))
                    {
                        return -EIO;
                    }
                    break;
                }
                case 1:
                {
                    if(buf[i]>='1' && buf[i]<='9')
                    {
                        info->SocketPort = info->SocketPort*10 + buf[i] - '0';
                    }
                    if(buf[i]=='\n')
                    {
                        status = 2;
                    }
                break;
                }
                default:break;
            }
            i = (i+1)%64;
        }
        timeout--;
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
    }
    if(status==2)
    {
        ret = WiFiBtDevSetRPTM();
        if(ESUCCESS != ret) return ret;
        printf("IP:%s - SocketPort:%d\r\n", info->IP, info->SocketPort);
        return ESUCCESS;
    }
    return -EIO;
}

int WiFiBtDevDisconnect(ConnectInfo info)
{
    int ret = -EIO;
    char skcls[32];
    sprintf(skcls, "AT+SKCLS=%d\r\n", info.SocketPort);
    xprintf("%s", skcls);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)skcls, strlen(skcls));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 100);
    return ret;
}

int WiFiBtDevWrite(ConnectInfo info, unsigned char *buf, unsigned int length)
{
    if(NULL == buf) return -EINVAL;
    if(0 == length) return -EINVAL;
    
    int ret = -EIO;
    char sksnd[32];
    sprintf(sksnd, "AT+SKSND=%d,%d\r\n", info.SocketPort, length);
    xprintf("%s", sksnd);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)sksnd, strlen(sksnd));
    if(ESUCCESS != ret) return ret;
    
    unsigned char tmp[32] = {0};
    unsigned char i = 0, step = 0;
    unsigned int nDataLength = 0;
    unsigned int timeout = 1000;
    while(step != 3 && timeout != 0)
    {
        if(pWiFiBtDev->Read(pWiFiBtDev, &tmp[i], 1) == 1)
        {
            switch(step)
            {
                case 0:
                {
                    if(strstr((char*)tmp, "+OK="))
                    {
                        step = 1;
                    }
                    break;
                }
                case 1:
                {
                    char c = tmp[i];
                    if(c >= '0' && c <= '9')
                    {
                        nDataLength = nDataLength * 10 + c - '0';
                    }
                    if(c == '\r')
                    {
                        step = 2;
                    }
                    break;
                }
                case 2:
                {
                    if(strstr((char*)tmp, "\r\n\r\n"))
                    {
                        step = 3;
                    }
                    break;
                }
                default:break;
            }
            i++;
        }
        timeout--;
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
    }
    
    if(step==3)
    {
        if(nDataLength < length)
            length = nDataLength;
        ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)buf, length);
        if(ESUCCESS != ret) return -EIO;
    }
    
    return ESUCCESS;
}

int WiFiBtDevRead(ConnectInfo info, unsigned char *buf, unsigned int length)
{
    if(NULL == buf) return -EINVAL;
    if(0 == length) return -EINVAL;

    if(gNetBuffer->info.nValidLength < length)
        length = gNetBuffer->info.nValidLength;
    if(gNetBuffer->Read(gNetBuffer, (unsigned char*)buf, length) != length)
        return -EIO;
    
    return (int)length;
}
