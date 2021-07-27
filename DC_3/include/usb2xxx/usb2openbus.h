/**
  ******************************************************************************
  * @file    usb2openbus.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2openbus��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2OPENBUS_H_
#define __USB2OPENBUS_H_

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
#define OB_SUCCESS             (0)   //����ִ�гɹ�
#define OB_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define OB_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define OB_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define OB_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��

//ͨ������
#define OB_CH0             (0)
#define OB_CH1             (1)
#define OB_CH2             (2)
#define OB_CH3             (3)
#define OB_CH4             (4)
#define OB_CH5             (5)
#define OB_CH6             (6)
#define OB_CH7             (7)
#define OB_CH8             (8)
#define OB_CH9             (9)
#define OB_CH10            (10)
#define OB_CH11            (11)
#define OB_CH12            (12)
#define OB_CH13            (13)
#define OB_CH14            (14)
#define OB_CH15            (15)
//�ڲ��������趨��
#define OB_INTERNAL_NOPULL        0x00    //û����������
#define OB_INTERNAL_PULL          0x01    //ʹ������,оƬ�ڲ��������������ܻ��޷���������������ʹ���ⲿ����

//������������
typedef struct _OB_DATA{
    char   Id;          //ID,��3bit��Ч
    unsigned int Data;  //32bit
}OB_DATA,*POB_DATA;

#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  ��ʼ������Openbus
  * @param  DevHandle �豸������,ͨ������USB_ScanDevice�������
  * @param  Channel ����ͨ���ţ�ȡֵ0~7,P0��Ӧͨ��0��Data���ţ�P1��Ӧͨ��0��Clk���ţ��Դ�����
  * @param  EnInternalPull �Ƿ�ʹ�������ڲ��������裬��������Ϊ��©ģʽ����Ҫ������������������ߵ�ƽ����û������������裬����ʹ���ڲ��������裬1-ʹ���ڲ�������0-��ʹ���ڲ�����
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI OB_Init(int DevHandle, unsigned char Channel,unsigned char EnInternalPull);

/**
  * @brief  Openbusд����
  * @param  DevHandle �豸������,ͨ������USB_ScanDevice�������
  * @param  Channel ����ͨ���ţ�ȡֵ0~7,P0��Ӧͨ��0��Data���ţ�P1��Ӧͨ��0��Clk���ţ��Դ�����
  * @param  pWriteData д���ݽṹ��ָ��
  * @param  WriteDataLen д���ݳ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI OB_Write(int DevHandle, unsigned char Channel,OB_DATA *pWriteData,unsigned int WriteDataLen);

/**
  * @brief  Openbus������
  * @param  DevHandle �豸������,ͨ������USB_ScanDevice�������
  * @param  Channel ����ͨ���ţ�ȡֵ0~7,P0��Ӧͨ��0��Data���ţ�P1��Ӧͨ��0��Clk���ţ��Դ�����
  * @param  pReadData �����ݽṹ��ָ�룬�����ݵ�ʱ����Ҫ�ȷ���ID�������ڵ��øú�����ʱ����Ҫ�����ID��Ϣ�����ݣ�Ȼ��������λ�ȡÿ��ID��Ӧ������
  * @param  ReadDataLen �����ݳ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI OB_Read(int DevHandle, unsigned char Channel,OB_DATA *pReadData,unsigned int ReadDataLen);


#ifdef __cplusplus
}
#endif
#endif
