/**
  ******************************************************************************
  * @file    usb2lin.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2lin��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2LIN_H_
#define __USB2LIN_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

//���庯�����ش������
#define LIN_SUCCESS             (0)   //����ִ�гɹ�
#define LIN_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define LIN_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define LIN_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define LIN_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
#define LIN_ERR_CH_NO_INIT      (-5)  //��ͨ��δ��ʼ��
#define LIN_ERR_READ_DATA       (-6)  //LIN������ʧ��
//LIN��У��ģʽ
#define LIN_CHECK_MODE_STD     0
#define LIN_CHECK_MODE_EXT     1
#define LIN_CHECK_MODE_NONE    2
//BREAKλ������
#define LIN_BREAK_BITS_10    0x00
#define LIN_BREAK_BITS_11    0x20
//��������ģʽ
#define LIN_MASTER          1
#define LIN_SLAVE           0
//����ӻ�����ģʽ
#define LIN_SLAVE_WRITE     0
#define LIN_SLAVE_READ      1

//�����ʼ��LIN��ʼ����������
typedef struct _LIN_CONFIG{
  unsigned int  BaudRate;     //������,���20K
  unsigned char CheckMode;    //У��ģʽ��0-��׼У��ģʽ��1-��ǿУ��ģʽ������PID��
  unsigned char MasterMode;   //����ģʽ��0-��ģʽ��1-��ģʽ
  unsigned char BreakBits;    //Break���ȣ�0x00-10bit,0x20-11bit
}LIN_CONFIG,*PLIN_CONFIG;

//LIN�����շ�֡��ʽ����
typedef struct _LIN_MSG{
    unsigned char ID;       //ID��ȡֵ��Χ0~0x3F
    unsigned char DataLen;  //��������ʱ�����������ݵĳ��ȣ�����У�����ݣ���������ʱ�����ݵĳ��ȣ�����У������
    unsigned char Data[9];  //���ݴ洢��
}LIN_MSG,*PLIN_MSG;

#ifdef __cplusplus
extern "C"
{
#endif

int WINAPI LIN_Init(int DevHandle,unsigned char LINIndex,LIN_CONFIG *pConfig);
int WINAPI LIN_SendBreak(int DevHandle,unsigned char LINIndex);
int WINAPI LIN_Write(int DevHandle,unsigned char LINIndex,LIN_MSG *pLINMsg,unsigned int Len);
int WINAPI LIN_Read(int DevHandle,unsigned char LINIndex,LIN_MSG *pLINMsg,unsigned int Len);
int WINAPI LIN_SlaveSetIDMode(int DevHandle,unsigned char LINIndex,unsigned char IDMode,LIN_MSG *pLINMsg,unsigned int Len);
int WINAPI LIN_SlaveGetData(int DevHandle,unsigned char LINIndex,LIN_MSG *pLINMsg);

#ifdef __cplusplus
}
#endif
#endif

