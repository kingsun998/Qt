/**
  ******************************************************************************
  * @file    uart_sniffer.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   uart sniffer��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __UART_SNIFFER_H_
#define __UART_SNIFFER_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

typedef enum
{ 
    IDLE = 0x00,
    START,
    DATA,
    PARITY,
    STOP,
}UART_DATA_TYPE;

typedef unsigned short UART_DATA;

typedef struct
{
    int BaudRate;
    int WordLength;
    int StopBits;
    int Parity;
}UART_SNIFFER_CONFIG;

//������UART���ݺ�Ļص�����
typedef  int (WINAPI UART_GET_DATA_HANDLE)(int DevHandle,int Channel,UART_DATA *pUARTData,int UartDataNum);
//ʶ�𵽲����ʺ�Ļص�����
typedef  int (WINAPI UART_GET_BAUDRATE_HANDLE)(int DevHandle,int Channel,int BaudRateHz);
//���庯�����ش������
#define SUCCESS             (0)   //����ִ�гɹ�
#define ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define ERR_CMD_FAIL        (-4)  //����ִ��ʧ��

#ifdef __cplusplus
extern "C"
{
#endif
    int WINAPI UART_SnifferGetBaudRate(int DevHandle,int Channel,unsigned int SampleRateHz,UART_GET_BAUDRATE_HANDLE *pGetBaudRateHandle);
    int WINAPI UART_SnifferStart(int DevHandle,unsigned char ChannelMask,UART_SNIFFER_CONFIG *pUartSnifferConfig,UART_GET_DATA_HANDLE *pGetUartDataHandle);
    int WINAPI UART_SnifferStop(int DevHandle);
#ifdef __cplusplus
}
#endif

#endif
