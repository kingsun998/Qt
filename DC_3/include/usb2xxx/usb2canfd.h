/**
  ******************************************************************************
  * @file    usb2canfd.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2canfd��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2CANFD_H_
#define __USB2CANFD_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif
//��������ֵ������Ϣ����
#define CANFD_SUCCESS             (0)   //����ִ�гɹ�
#define CANFD_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define CANFD_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define CANFD_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define CANFD_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
//CANFD_MSG.ID����
#define CANFD_MSG_FLAG_RTR      (1<<30)
#define CANFD_MSG_FLAG_IDE      (1<<31)
#define CANFD_MSG_FLAG_ID_MASK  (0x1FFFFFFF)
//CANFD_MSG.Flags����
#define CANFD_MSG_FLAG_BRS      (1<<0)
#define CANFD_MSG_FLAG_ESI      (1<<1)
#define CANFD_MSG_FLAG_FDF      (1<<2)
#define CANFD_MSG_FLAG_RXD      (1<<7)
//CANFD_DIAGNOSTIC.Flags����
#define CANFD_DIAGNOSTIC_FLAG_NBIT0_ERR     (0x0001)//�ڷ��ͱ��ģ���Ӧ��λ�����������־����ر�־���ڼ䣬����Ҫ�������Ե�ƽ���߼�ֵΪ0�����ݻ��ʶ��λ���������ӵ�����ֵΪ���ԡ�
#define CANFD_DIAGNOSTIC_FLAG_NBIT1_ERR     (0x0002)//�ڷ��ͱ��ģ��ٲ��ֶγ��⣩�ڼ䣬����Ҫ�������Ե�ƽ���߼�ֵΪ1��λ���������ӵ�������ֵΪ���ԡ�
#define CANFD_DIAGNOSTIC_FLAG_NACK_ERR      (0x0004)//���ͱ���δӦ��
#define CANFD_DIAGNOSTIC_FLAG_NFORM_ERR     (0x0008)//���ձ��ĵĹ̶���ʽ���ָ�ʽ����
#define CANFD_DIAGNOSTIC_FLAG_NSTUFF_ERR    (0x0010)//�ڽ��ձ��ĵ�һ�����У������а�����5���������λ���������в���������������С�
#define CANFD_DIAGNOSTIC_FLAG_NCRC_ERR      (0x0020)//���յı��ĵ�CRCУ��Ͳ���ȷ�����뱨�ĵ�CRC��ͨ�����յ������ݼ���õ���CRC��ƥ�䡣
#define CANFD_DIAGNOSTIC_FLAG_TXBO_ERR      (0x0080)//������������״̬�����Զ��ָ�����
#define CANFD_DIAGNOSTIC_FLAG_DBIT0_ERR     (0x0100)//��NBIT0_ERR
#define CANFD_DIAGNOSTIC_FLAG_DBIT1_ERR     (0x0200)//��NBIT1_ERR
#define CANFD_DIAGNOSTIC_FLAG_DFORM_ERR     (0x0800)//��NFORM_ERR
#define CANFD_DIAGNOSTIC_FLAG_DSTUFF_ERR    (0x1000)//��NSTUFF_ERR
#define CANFD_DIAGNOSTIC_FLAG_DCRC_ERR      (0x2000)//��NCRC_ERR
#define CANFD_DIAGNOSTIC_FLAG_ESI_ERR       (0x4000)//���յ�CAN FD���ĵ�ESI��־��1
#define CANFD_DIAGNOSTIC_FLAG_DLC_MISMATCH  (0x8000)//DLC��ƥ��,�ڷ��ͻ�����ڼ䣬ָ����DLC����FIFOԪ�ص�PLSIZE
//CANFD_BUS_ERROR.Flags����
#define CANFD_BUS_ERROR_FLAG_TX_RX_WARNING   (0x01)
#define CANFD_BUS_ERROR_FLAG_RX_WARNING      (0x02)
#define CANFD_BUS_ERROR_FLAG_TX_WARNING      (0x04)
#define CANFD_BUS_ERROR_FLAG_RX_BUS_PASSIVE  (0x08)
#define CANFD_BUS_ERROR_FLAG_TX_BUS_PASSIVE  (0x10)
#define CANFD_BUS_ERROR_FLAG_TX_BUS_OFF      (0x20)
//1.CANFD��Ϣ֡���������Ͷ���
typedef  struct  _CANFD_MSG
{
    unsigned int    ID;           //����ID,bit[30]-RTR,bit[31]-IDE,bit[28..0]-ID
    unsigned char   DLC;          //�����ֽڳ��ȣ�������Ϊ-0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64
    unsigned char   Flags;        //bit[0]-BRS,bit[1]-ESI,bit[2]-FDF,bit[6..5]-Channel,bit[7]-RXD
    unsigned char   __Res0;       //����
    unsigned char   __Res1;       //����
    unsigned int    TimeStamp;    //֡���ջ��߷���ʱ��ʱ�������λΪus
    unsigned char   Data[64];     //���ĵ����ݡ�
}CANFD_MSG;

//2.CANFD��ʼ�������������Ͷ���
typedef  struct  _CANFD_INIT_CONFIG
{
    unsigned char Mode; //0-����ģʽ��1-�Է�����ģʽ
    unsigned char ISOCRCEnable;//0-��ֹISO CRC,1-ʹ��ISO CRC
    unsigned char RetrySend;//0-��ֹ�ط���1-�������ط�
    unsigned char ResEnable;//0-�������ڲ�120ŷ�ն˵��裬1-�����ڲ�120ŷ�ն˵���
    //�����ʲ���������TCANLINPro�������Ĳ����ʼ��㹤�߼���
    //�ٲöβ����ʲ���,������=40M/NBT_BRP*(1+NBT_SEG1+NBT_SEG2)
    unsigned char NBT_BRP;
    unsigned char NBT_SEG1;
    unsigned char NBT_SEG2;
    unsigned char NBT_SJW;
    //���ݶβ����ʲ���,������=40M/DBT_BRP*(1+DBT_SEG1+DBT_SEG2)
    unsigned char DBT_BRP;
    unsigned char DBT_SEG1;
    unsigned char DBT_SEG2;
    unsigned char DBT_SJW;
    unsigned char __Res0[8];
}CANFD_INIT_CONFIG;
//3.CANFD���֡��Ϣ�ṹ�嶨��
typedef struct _CANFD_DIAGNOSTIC {
    unsigned char NREC;//��Ʊ����ʽ��մ������
    unsigned char NTEC;//��Ʊ����ʷ��ʹ������
    unsigned char DREC;//���ݱ����ʽ��մ������
    unsigned char DTEC;//���ݱ����ʷ��ʹ������
    unsigned short ErrorFreeMsgCount;//�޴���֡����
    unsigned short Flags;//�ο���ϱ�־����
}CANFD_DIAGNOSTIC;
//4.CANFD���ߴ�����Ϣ�ṹ�嶨��
typedef struct _CANFD_BUS_ERROR {
    unsigned char TEC;//���ʹ������
    unsigned char REC;//���մ������
    unsigned char Flags;//�ο����ߴ����־����
    unsigned char __Res0;
}CANFD_BUS_ERROR;
//5.CAN �˲��������������Ͷ���
typedef struct _CANFD_FILTER_CONFIG{
    unsigned char   Enable;   //ʹ�ܸù�������1-ʹ�ܣ�0-��ֹ
    unsigned char   Index;    //�����������ţ�ȡֵ��ΧΪ0��31
    unsigned char __Res0;
    unsigned char __Res1;
    unsigned int    ID_Accept;//������ID,bit[28..0]Ϊ��ЧIDλ��bit[31]ΪIDE
    unsigned int    ID_Mask;  //�����룬��Ӧbitλ��Ϊ1������Ҫ�Աȶ�Ӧ������bitλ����ͬ�Ž���
}CANFD_FILTER_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif

int WINAPI CANFD_Init(int DevHandle, unsigned char CANIndex, CANFD_INIT_CONFIG *pCanConfig);
int WINAPI CANFD_StartGetMsg(int DevHandle, unsigned char CANIndex);
int WINAPI CANFD_StopGetMsg(int DevHandle, unsigned char CANIndex);
int WINAPI CANFD_SendMsg(int DevHandle, unsigned char CANIndex, CANFD_MSG *pCanSendMsg,unsigned int SendMsgNum);
int WINAPI CANFD_GetMsg(int DevHandle, unsigned char CANIndex, CANFD_MSG *pCanGetMsg,int BufferSize);
int WINAPI CANFD_SetFilter(int DevHandle, unsigned char CANIndex, CANFD_FILTER_CONFIG *pCanFilter,unsigned char Len);
int WINAPI CANFD_GetDiagnostic(int DevHandle, unsigned char CANIndex, CANFD_DIAGNOSTIC *pCanDiagnostic);
int WINAPI CANFD_GetBusError(int DevHandle, unsigned char CANIndex, CANFD_BUS_ERROR *pCanBusError);

int WINAPI CANFD_SetRelay(int DevHandle, unsigned char RelayState);
int WINAPI CANFD_SetRelayData(int DevHandle, CAN_RELAY_HEAD *pCANRelayHead, CAN_RELAY_DATA *pCANRelayData);
int WINAPI CANFD_GetRelayData(int DevHandle, CAN_RELAY_HEAD *pCANRelayHead, CAN_RELAY_DATA *pCANRelayData);
#ifdef __cplusplus
}
#endif
#endif

