/**
  ******************************************************************************
  * @file    usb2cnt.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2cnt��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2CNT_H_
#define __USB2CNT_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

//�����ʼ��Counter����������
typedef struct _CNT_CONFIG{
  unsigned char   CounterMode;      //����ģʽ��0-Up,1-Down
  unsigned char   CounterPolarity;  //�������ԣ�0-Rising��1-Falling��2-BothEdge
  unsigned char   CounterBitWide;   //����λ��16-16bitλ��32-32bitλ��
  unsigned char   CounterPinMode;   //��������������ģʽ��0-�������룬1-�������룬2-��������
}CNT_CONFIG,*PCNT_CONFIG;

//���庯�����ش������
#define CNT_SUCCESS             (0)   //����ִ�гɹ�
#define CNT_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define CNT_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define CNT_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define CNT_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
//������ģʽ
#define COUNTER_MODE_UP     0
#define COUNTER_MODE_DOWN   1
//��������������
#define COUNTER_POL_RISING     0
#define COUNTER_POL_FALLING    1
#define COUNTER_POL_BOTHEDGE   2
//���������
#define COUNTER_BITS16        16
#define COUNTER_BITS32        32
//��������������ģʽ
#define  COUNTER_PIN_NOPULL     0x00
#define  COUNTER_PIN_UP         0x01
#define  COUNTER_PIN_DOWN       0x02
//������ͨ������
#define COUNTER_CH0             0x01
#define COUNTER_CH1             0x02
#define COUNTER_CH2             0x04
#define COUNTER_CH3             0x08

#ifdef __cplusplus
extern "C"
{
#endif

int WINAPI CNT_Init(int DevHandle, unsigned char ChannelMask, PCNT_CONFIG pConfig);
int WINAPI CNT_Start(int DevHandle, unsigned char ChannelMask);
int WINAPI CNT_Stop(int DevHandle, unsigned char ChannelMask);
int WINAPI CNT_SetValue(int DevHandle, unsigned char ChannelMask,unsigned int *pCntValues);
int WINAPI CNT_GetValue(int DevHandle, unsigned char ChannelMask,unsigned int *pCntValues);

#ifdef __cplusplus
}
#endif
#endif
