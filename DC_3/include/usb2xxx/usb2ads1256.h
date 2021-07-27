/**
  ******************************************************************************
  * @file    usb2ads1256.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2ads1256��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2ADS1256_H_
#define __USB2ADS1256_H_

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
#define ADS1256_SUCCESS             (0)   //����ִ�гɹ�
#define ADS1256_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define ADS1256_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define ADS1256_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define ADS1256_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
#define ADS1256_ERR_CH_NO_INIT      (-5)  //��ͨ��δ��ʼ��

//����ADS������
#define ADS1256_DRATE_30000SPS   0xF0 
#define ADS1256_DRATE_15000SPS   0xE0 
#define ADS1256_DRATE_7500SPS   0xD0 
#define ADS1256_DRATE_3750SPS   0xC0 
#define ADS1256_DRATE_2000SPS   0xB0 
#define ADS1256_DRATE_1000SPS   0xA1 
#define ADS1256_DRATE_500SPS    0x92 
#define ADS1256_DRATE_100SPS    0x82 
#define ADS1256_DRATE_60SPS     0x72 
#define ADS1256_DRATE_50SPS     0x63 
#define ADS1256_DRATE_30SPS     0x53 
#define ADS1256_DRATE_25SPS     0x43 
#define ADS1256_DRATE_15SPS     0x33 
#define ADS1256_DRATE_10SPS     0x23 
#define ADS1256_DRATE_5SPS      0x13 
#define ADS1256_DRATE_2_5SPS    0x03

//�����ʼ��ADS1256����������
typedef struct _ADS1256_CONFIG{
  unsigned short  SampleRate;   //������
  unsigned char   PGA;          //�ڲ����棬2^n
  unsigned char   BufferEn;     //1��ʹ��Buffer,0����ֹBuffer
  unsigned int    SampleMode;   //����bit����һ��ͨ����00����ʹ�ø�ͨ����01����ͨ��Ϊ����ģʽ��10����ͨ��Ϊ��ָ�����11����ͨ��Ϊ�������
}ADS1256_CONFIG,*PADS1256_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  ��ʼ������ADS1256
  * @param  DevHandle �豸������
  * @param  Channel ��Ҫ���õ�ADSͨ���ţ�ȡֵ0����1
  * @param  pConfig ADS1256���ò���ָ��
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI ADS1256_Init(int DevHandle,char Channel,ADS1256_CONFIG *pConfig);
/**
  * @brief  ����ADSת�������øú��������������Զ���ȡADSת������
  * @param  DevHandle �豸������
  * @param  Channel ��Ҫ������ADSͨ���ţ�ȡֵ0����1
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI ADS1256_Start(int DevHandle,char Channel);
/**
  * @brief  ����ADC����ת��ģʽ
  * @param  DevHandle �豸������
  * @param  Channel ��Ҫ��ȡ���ݵ�ADSͨ���ţ�ȡֵ0����1
  * @param  pData ���ݴ洢�������׵�ַ
  * @retval ����ִ��״̬��С��0����ִ�г�������0Ϊʵ�ʲɼ�����ADC����
  */
int WINAPI ADS1256_Read(int DevHandle,char Channel,int *pData);
/**
  * @brief  ֹͣADSת�������øú��������������Զ�ֹͣADSת������
  * @param  DevHandle �豸������
  * @param  Channel ��Ҫ������ADSͨ���ţ�ȡֵ0����1
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI ADS1256_Stop(int DevHandle,char Channel);


#ifdef __cplusplus
}
#endif

#endif

