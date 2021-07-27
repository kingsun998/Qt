/**
  ******************************************************************************
  * @file    usb2nand.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2nand��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2NAND_H_
#define __USB2NAND_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

//����NAND����ʱ�����
typedef struct _NAND_TIMING_COMFIG{
  uint32_t FSMC_SetupTime;
  uint32_t FSMC_WaitSetupTime;
  uint32_t FSMC_HoldSetupTime;
  uint32_t FSMC_HiZSetupTime;
}NAND_TIMING_COMFIG,*PNAND_TIMING_COMFIG;

//���庯�����ش������
#define NAND_SUCCESS             (0)   //����ִ�гɹ�
#define NAND_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define NAND_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define NAND_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define NAND_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��

#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  ��ʼ������NAND�ӿڣ��ú����������
  * @param  DevHandle �豸������
  * @param  ChipIndex NANDоƬƬѡ�ţ�ȡֵ0����1
  * @param  PowerLevel NAND�ӿڵ�ѹ��1-1.8V��3-3.3V
  * @param  pTimeConfig ��NAND���ж�д������ʱ�����
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_Init(int DevHandle,int ChipIndex,unsigned char PowerLevel,PNAND_TIMING_COMFIG pTimeConfig);

//NAND�����ײ㺯����������Щ���������Ͽ��Զ�д�����κ�NANDоƬ���ײ�������������Զ����Ƭѡ�źţ���Ҫ�������Ƭѡ�ź����
/**
  * @brief  ����Ƭѡ�ź�����͵�ƽ
  * @param  DevHandle �豸������
  * @param  ChipIndex NANDоƬƬѡ�ţ�ȡֵ0����1
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_Select(int DevHandle,int ChipIndex);

/**
  * @brief  ����Ƭѡ�ź�����ߵ�ƽ
  * @param  DevHandle �豸������
  * @param  ChipIndex NANDоƬƬѡ�ţ�ȡֵ0����1
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_Unselect(int DevHandle,int ChipIndex);

/**
  * @brief  NAND�ӿ������������
  * @param  DevHandle �豸������
  * @param  pCmds ����洢���ݻ�����
  * @param  CmdsLen ��Ҫ���͵�������
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_SendCmd(int DevHandle,int ChipIndex,unsigned char *pCmds,unsigned char CmdsLen);

/**
  * @brief  NAND�ӿ������ַ����
  * @param  DevHandle �豸������
  * @param  pAddrs ��ַ�洢���ݻ�����
  * @param  AddrsLen ��Ҫ���͵ĵ�ַ�����ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_SendAddr(int DevHandle,int ChipIndex,unsigned char *pAddrs,unsigned char AddrsLen);

/**
  * @brief  NAND�ӿ��������
  * @param  DevHandle �豸������
  * @param  pWriteData ���ݴ洢������
  * @param  WriteDataLen ��Ҫ���͵������ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_WriteData(int DevHandle,int ChipIndex,unsigned char *pWriteData,unsigned int WriteDataLen);

/**
  * @brief  NAND�ӿڶ�ȡ����
  * @param  DevHandle �豸������
  * @param  pReadData ���ݴ洢������
  * @param  ReadDataLen ��Ҫ��ȡ�������ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_ReadData(int DevHandle,int ChipIndex,unsigned char *pReadData,unsigned int ReadDataLen);
/**
  * @brief  �ȴ�NAND�ӿڵ�RB�źű��
  * @param  DevHandle �豸������
  * @param  ChipIndex ��Ҫ�ȴ���RBͨ���ţ�0-RB0��1-RB1
  * @param  TimeOutOfMs �ȴ�RB�źű�ߵĳ�ʱʱ�䣬��λΪ���룬������Ϊ0���ʾһֱ�ȴ���ֱ����ߺ�ŷ���
  * @retval ����ִ��״̬��С��0����ִ�г���,0-�ȴ���ʱ��RBΪ�ͣ�1-��ʱʱ�䷶Χ�ڼ�⵽RBΪ��
  */
int WINAPI NAND_WaitReady(int DevHandle,int ChipIndex,unsigned int TimeOutOfMs);



//NAND�����߲㺯��
/**
  * @brief  ����RESET(0xFF)����ȴ�RB�źű��
  * @param  DevHandle �豸������
  * @param  ChipIndex CE��RB�Զ����ͨ���ţ�0-CE0,RB0;1-CE1,RB1
  * @retval ����ִ��״̬��С��0����ִ�г���,0-�ȴ���ʱ��RBΪ�ͣ�1-��ʱʱ�䷶Χ�ڼ�⵽RB���
  */
int WINAPI NAND_Reset(int DevHandle,int ChipIndex);

/**
  * @brief  ��ȡNAND FlashоƬ��״̬��Ϣ
  * @param  DevHandle �豸������
  * @param  ChipIndex �Զ������NANDоƬƬѡ�ţ�ȡֵ0����1
  * @param  pStatus ״̬��Ϣ�洢ָ��
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_ReadStatus(int DevHandle,int ChipIndex,unsigned int *pStatus);


/**
  * @brief  ��ȡNAND FlashоƬID��Ϣ
  * @param  DevHandle �豸������
  * @param  ChipIndex �Զ������NANDоƬƬѡ�ţ�ȡֵ0����1
  * @param  IDAddr ��ȡID�ĵ�ַ��Ϣ
  * @param  pID оƬID�洢�������׵�ַ
  * @param  IDLen ��ȡID�����ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_ReadID(int DevHandle,int ChipIndex,unsigned char IDAddr,unsigned char *pID,unsigned char IDLen);

/**
  * @brief  ��NAND Flashд�����ݣ����鰴����ҳд��Ȼ���ٽ��ú������ص�ECCд�뱸�������Ա��Ժ��ȡ��ʱ�򽫶������ݵ�ECC��д��ECC���Աȣ�ʵ�ֶ����ݵ�У��
  * @param  DevHandle �豸������
  * @param  ChipIndex NANDоƬƬѡ�ţ�ȡֵ0����1
  * @param  pCmds д���ݵ�����
  * @param  CmdsLen д���������ֽ���
  * @param  pAddrs д���ݵĵ�ַ
  * @param  AddrLen д���ݵĵ�ַ�ֽ���
  * @param  pWriteData ��д��NAND FlashоƬ�����ݻ������׵�ַ
  * @param  NumByteToWrite ��д��NAND FlashоƬ�������ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_WritePage(int DevHandle,int ChipIndex,unsigned char *pCmds,unsigned char CmdsLen,unsigned char *pAddrs,unsigned char AddrLen,unsigned char *pWriteData,int NumByteToWrite);
/**
  * @brief  ��NAND Flash��������
  * @param  DevHandle �豸������
  * @param  ChipIndex NANDоƬƬѡ�ţ�ȡֵ0����1
  * @param  pCmds �����ݵ�����
  * @param  CmdsLen �����������ֽ���
  * @param  pAddrs �����ݵĵ�ַ
  * @param  AddrLen �����ݵĵ�ַ�ֽ���
  * @param  pReadData �洢�������ݵĻ������׵�ַ
  * @param  NumByteToRead �������������ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_ReadPage (int DevHandle,int ChipIndex, unsigned char *pCmds,unsigned char CmdsLen,unsigned char *pAddrs,unsigned char AddrLen,unsigned char *pReadData, int NumByteToRead);
/**
  * @brief  �������NAND Flash���ݣ��������������ݱ��0xFF
  * @param  DevHandle �豸������
  * @param  ChipIndex NANDоƬƬѡ�ţ�ȡֵ0����1
  * @param  pCmds �������ݵ�����
  * @param  CmdsLen �������������ֽ���
  * @param  pAddrs �������ݵĵ�ַ
  * @param  AddrLen �������ݵĵ�ַ�ֽ���
  * @retval ����ִ��״̬��С��0����ִ�г���
  */
int WINAPI NAND_EraseBlock(int DevHandle,int ChipIndex,unsigned char *pCmds,unsigned char CmdsLen,unsigned char *pAddrs,unsigned char AddrLen);


#ifdef __cplusplus
}
#endif

#endif
