/**
  ******************************************************************************
  * @file    usb2dac.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2dac��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2DAC_H_
#define __USB2DAC_H_

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
#define DAC_SUCCESS             (0)   //����ִ�гɹ�
#define DAC_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define DAC_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define DAC_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define DAC_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
#define DAC_ERR_CH_NO_INIT      (-5)  //��ͨ��δ��ʼ��

#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  ��ʼ������DAC������ź�
  * @param  DevHandle �豸������
  * @param  CycleUs �������ʱ��������λΪ0.25us�����������ʱ��=0.25*CycleUs*WaveDataNum us
  * @param  pWaveData ����������ݻ�����ָ�룬�������ֵΪ0xFFF,��Ӧ��ѹΪ3.3V
  * @param  WaveDataNum ��������ݵ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI DAC_Init(int DevHandle,unsigned short CycleUs,unsigned short *pWaveData,unsigned short WaveDataNum);
/**
  * @brief  ����DAC������ź�
  * @param  DevHandle �豸������
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI DAC_Start(int DevHandle);
/**
  * @brief  ֹͣDAC����ź�
  * @param  DevHandle �豸������
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI DAC_Stop(int DevHandle);

#ifdef __cplusplus
}
#endif

#endif

