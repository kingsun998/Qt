/**
  ******************************************************************************
  * @file    usb2can.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2can��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.usbxyz.com">http://www.usbxyz.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef __CAN_BOOTLOADER_H_
#define __CAN_BOOTLOADER_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif


//1.�������ش�����붨��
#define CAN_SUCCESS             (0)   //����ִ�гɹ�
#define CAN_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define CAN_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define CAN_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define CAN_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��

#define CAN_BOOT_ERR_CONFIG       (-30) //�����豸����
#define CAN_BOOT_ERR_SEND         (-31) //�������ݳ���
#define CAN_BOOT_ERR_TIME_OUT     (-32) //��ʱ����
#define CAN_BOOT_ERR_CMD          (-33) //ִ������ʧ��
#define CAN_BOOT_ERR_BAUD         (-34) //�����ʲ����Զ���ȡʧ��
#define CAN_BOOT_ERR_BUFFER       (-35) //���豸���ؽ������ݻ�������СΪ0
#define CAN_BOOT_ERR_DSIZE        (-36) //���δ���������������4093�ֽ�

#define CAN_BOOT_ERR_ERASE          1   //�̼���������
#define CAN_BOOT_ERR_ERASE_IN_APP   2   //��ǰģʽΪAPP�����ܲ����̼�
#define CAN_BOOT_ERR_WRITE_OUTRANGE 3   //��ǰ��ַ�����������ĵ�ַ��Χ
#define CAN_BOOT_ERR_WRITE_IN_APP   4   //��ǰģʽ����д��̼�����
#define CAN_BOOT_ERR_WRITE          5   //����д�����洢������
#define CAN_BOOT_ERR_WRITE_OUT_ADDR 6   //���ݳ��ȳ����˳���洢����Χ
#define CAN_BOOT_ERR_WRITE_CRC      7   //����CRCУ�����

//2.CAN Bootloader�̼�����
#define FW_TYPE_BOOT     0x55
#define FW_TYPE_APP      0xAA

//3.CAN Bootloader�����շ�ID����
#define ID_TYPE_EXT     0x01
#define ID_TYPE_STD     0x00

#ifdef __cplusplus
extern "C"
{
#endif

int WINAPI CAN_BOOT_Init(int DevHandle,int CANIndex,unsigned int SendDataID,unsigned int ReceiveDataID,unsigned char IDType,int BaudRate);
int WINAPI CAN_BOOT_GetFWInfo(int DevHandle,int CANIndex,unsigned char NAD,unsigned char *pFWType,unsigned int *pFWVersion,unsigned int TimeOutOfMs);
int WINAPI CAN_BOOT_EnterBootMode(int DevHandle,int CANIndex,unsigned char NAD);
int WINAPI CAN_BOOT_EraseApp(int DevHandle,int CANIndex,unsigned char NAD,unsigned int AppSizeOfByte,unsigned int TimeOutOfMs);
int WINAPI CAN_BOOT_SetAddrOffset(int DevHandle,int CANIndex,unsigned char NAD,unsigned int AddrOffset,unsigned short *pBufferSize,unsigned int TimeOutOfMs);
int WINAPI CAN_BOOT_SendAppData(int DevHandle,int CANIndex,unsigned char NAD,unsigned char *pData,unsigned short DataLen,unsigned short *pCRC16);
int WINAPI CAN_BOOT_WriteAppData(int DevHandle,int CANIndex,unsigned char NAD,unsigned short CRC16,unsigned int TimeOutOfMs);
int WINAPI CAN_BOOT_ExecuteApp(int DevHandle,int CANIndex,unsigned char NAD);

#ifdef __cplusplus
}
#endif
#endif
