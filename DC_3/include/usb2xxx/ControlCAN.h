/**
  ******************************************************************************
  * @file    ControlCAN.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-CAN�������ײ�������API��������.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef _CONTROLCAN_H_
#define _CONTROLCAN_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#ifndef WINAPI
#define WINAPI
#endif
#endif

//���������Ͷ���
#define VCI_USBCAN1     3
#define VCI_USBCAN2     4

//CAN������
#define ERR_CAN_OVERFLOW        0x0001  //CAN �������ڲ�FIFO���
#define ERR_CAN_ERRALARM        0x0002  //CAN ���������󱨾�
#define ERR_CAN_PASSIVE         0x0004  //CAN ��������������
#define ERR_CAN_LOSE            0x0008  //CAN �������ٲö�ʧ
#define ERR_CAN_BUSERR          0x0010  //CAN ���������ߴ���
#define ERR_CAN_BUSOFF          0x0020  //CAN ���������߹ر�

//ͨ�ô�����
#define ERR_DEVICEOPENED        0x0100  //�豸�Ѿ���
#define ERR_DEVICEOPEN          0x0200  //���豸����
#define ERR_DEVICENOTOPEN       0x0400  //�豸û�д�
#define ERR_BUFFEROVERFLOW      0x0800  //���������
#define ERR_DEVICENOTEXIST      0x1000  //���豸������
#define ERR_LOADKERNELDLL       0x2000  //װ�ض�̬��ʧ��
#define ERR_CMDFAILED           0x4000  //ִ������ʧ�ܴ�����
#define ERR_BUFFERCREATE        0x8000  //�ڴ治��

//�������÷���״ֵ̬
#define STATUS_OK   1
#define STATUS_ERR  0

//1.����ZLGCANϵ�нӿڿ���Ϣ���������͡�
typedef  struct  _VCI_BOARD_INFO{
    unsigned short  hw_Version;         //Ӳ���汾�ţ���16 ���Ʊ�ʾ������0x0100 ��ʾV1.00��
    unsigned short  fw_Version;         //�̼��汾�ţ���16 ���Ʊ�ʾ��
    unsigned short  dr_Version;         //��������汾�ţ���16 ���Ʊ�ʾ��
    unsigned short  in_Version;         //�ӿڿ�汾�ţ���16 ���Ʊ�ʾ��
    unsigned short  irq_Num;            //�忨��ʹ�õ��жϺš�
    unsigned char   can_Num;            //��ʾ�м�·CAN ͨ����
    unsigned char   str_Serial_Num[20]; //�˰忨�����кš�
    unsigned char   str_hw_Type[40];    //Ӳ�����ͣ����硰USBCAN V1.00����ע�⣺�����ַ�����������\0������
    unsigned short  Reserved[4];        //ϵͳ������
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;

//1.Ginkgoϵ�нӿڿ���Ϣ���������͡�
typedef  struct  _VCI_BOARD_INFO_EX{
    unsigned char   ProductName[32];    //Ӳ�����ƣ�ע�⣺�����ַ�����������\0����
    unsigned char   FirmwareVersion[4]; //�̼��汾
    unsigned char   HardwareVersion[4]; //Ӳ���汾
    unsigned char   SerialNumber[12];   //���������к�
} VCI_BOARD_INFO_EX,*PVCI_BOARD_INFO_EX;

//2.����CAN��Ϣ֡���������͡�
typedef  struct  _VCI_CAN_OBJ{
    unsigned int    ID;         //����ID��
    unsigned int    TimeStamp;  //���յ���Ϣ֡ʱ��ʱ���ʶ����CAN ��������ʼ����ʼ��ʱ��
    unsigned char   TimeFlag;   //�Ƿ�ʹ��ʱ���ʶ��Ϊ1 ʱTimeStamp ��Ч��TimeFlag ��TimeStamp ֻ�ڴ�֡Ϊ����֡ʱ�����塣
    unsigned char   SendType;   //����֡���ͣ�=0 ʱΪ�������ͣ�=1 ʱΪ���η��ͣ�=2 ʱΪ�Է����գ�=3 ʱΪ�����Է����գ�ֻ�ڴ�
                                //֡Ϊ����֡ʱ�����塣�����豸����ΪEG20T-CAN ʱ�����ͷ�ʽ��VCI_InitCan ��ͨ�����ã��˴���
                                //������Ч������Ϊ�Է�����ģʽ��EG20T-CAN ���ܴ������Ͻ������ݣ�ֻ���յ��Լ����������ݣ�
    unsigned char   RemoteFlag; //�Ƿ���Զ��֡
    unsigned char   ExternFlag; //�Ƿ�����չ֡
    unsigned char   DataLen;    //���ݳ���(<=8)����Data �ĳ��ȡ�
    unsigned char   Data[8];    //���ĵ����ݡ�
    unsigned char   Reserved[3];//ϵͳ������
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.����CAN������״̬���������͡�
typedef struct _VCI_CAN_STATUS{
    unsigned char   ErrInterrupt;   //�жϼ�¼���������������
    unsigned char   regMode;        //CAN ������ģʽ�Ĵ�����
    unsigned char   regStatus;      //CAN ������״̬�Ĵ�����
    unsigned char   regALCapture;   //CAN �������ٲö�ʧ�Ĵ�����
    unsigned char   regECCapture;   //CAN ����������Ĵ�����
    unsigned char   regEWLimit;     //CAN ���������󾯸����ƼĴ�����
    unsigned char   regRECounter;   //CAN ���������մ���Ĵ�����
    unsigned char   regTECounter;   //CAN ���������ʹ���Ĵ�����
    unsigned int    regESR;         //CAN ����������״̬�Ĵ�����
    unsigned int    regTSR;         //CAN ����������״̬�Ĵ���
    unsigned int    BufferSize;     //CAN ���������ջ�������С
    unsigned int    Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.���������Ϣ���������͡�
typedef struct _ERR_INFO{
    unsigned int    ErrCode;            //������
    unsigned char   Passive_ErrData[3]; //�������Ĵ���������������ʱ��ʾΪ��������Ĵ����ʶ���ݡ�
    unsigned char   ArLost_ErrData;     //�������Ĵ��������ٲö�ʧ����ʱ��ʾΪ�ٲö�ʧ����Ĵ����ʶ���ݡ�
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.�����ʼ��CAN����������
typedef struct _INIT_CONFIG{
    unsigned int    AccCode;    //������
    unsigned int    AccMask;    //������
    unsigned int    Reserved;   //����
    unsigned char   Filter;     //�˲���ʽ,0-˫�˲����������е�֡��1-���˲�������AccCode��AccMask�����ù����� 
    unsigned char   Timing0;    //��ʱ��0��BTR0����
    unsigned char   Timing1;    //��ʱ��1��BTR1����
    unsigned char   Mode;       //ģʽ��0-����ģʽ��1-����ģʽ��2-�Է�����Ĭ�ϣ������λ����Ϊ1�����簴λ����0x80�����ʾ����120ŷ���ն˵��赽CAN���ߣ����򲻽��롣
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//6.������CAN �˲���������
typedef struct _VCI_FILTER_CONFIG{
    unsigned char   Enable;         //ʹ�ܸù�������1-ʹ�ܣ�0-��ֹ
    unsigned char   FilterIndex;    //�����������ţ�ȡֵ��ΧΪ0��13
    unsigned char   FilterMode;     //������ģʽ��0-����λģʽ��1-��ʶ���б�ģʽ
    unsigned char   ExtFrame;       //���˵�֡���ͱ�־��Ϊ1 ����Ҫ���˵�Ϊ��չ֡��Ϊ0 ����Ҫ���˵�Ϊ��׼֡��
    unsigned int    ID_Std_Ext;     //������ID
    unsigned int    ID_IDE;         //������IDE
    unsigned int    ID_RTR;         //������RTR
    unsigned int    MASK_Std_Ext;   //������ID������ֻ���ڹ�����ģʽΪ����λģʽʱ����
    unsigned int    MASK_IDE;       //������IDE������ֻ���ڹ�����ģʽΪ����λģʽʱ����
    unsigned int    MASK_RTR;       //������RTR������ֻ���ڹ�����ģʽΪ����λģʽʱ����
    unsigned int    Reserved;       //ϵͳ����
} VCI_FILTER_CONFIG,*PVCI_FILTER_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __OS_ANDROID
unsigned int WINAPI VCI_ScanDevice(int *fd,int devNum);
#else
unsigned int WINAPI VCI_ScanDevice(unsigned char NeedInit);
#endif
#ifdef __OS_ANDROID
unsigned int WINAPI VCI_OpenDevice(unsigned int DevType, unsigned int DevIndex,int *pFd,int DevNum);
#else
unsigned int WINAPI VCI_OpenDevice(unsigned int DevType, unsigned int DevIndex,unsigned int Reserved);
#endif
unsigned int WINAPI VCI_CloseDevice(unsigned int DevType,unsigned int DevIndex);
unsigned int WINAPI VCI_InitCAN(unsigned int DevType, unsigned int DevIndex, unsigned int CANIndex, PVCI_INIT_CONFIG pInitConfig);

unsigned int WINAPI VCI_ReadBoardInfo(unsigned int DevType,unsigned int DevIndex,PVCI_BOARD_INFO pInfo);
unsigned int WINAPI VCI_ReadErrInfo(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,PVCI_ERR_INFO pErrInfo);
unsigned int WINAPI VCI_ReadCANStatus(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,PVCI_CAN_STATUS pCANStatus);

unsigned int WINAPI VCI_GetReference(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,unsigned int RefType,void *pData);
unsigned int WINAPI VCI_SetReference(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,unsigned int RefType,void *pData);
unsigned int WINAPI VCI_SetFilter(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,PVCI_FILTER_CONFIG pFilter);

unsigned int WINAPI VCI_GetReceiveNum(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex);
unsigned int WINAPI VCI_ClearBuffer(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex);

unsigned int WINAPI VCI_StartCAN(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex);
unsigned int WINAPI VCI_ResetCAN(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex);

unsigned int WINAPI VCI_Transmit(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,PVCI_CAN_OBJ pSend,unsigned int Len);
unsigned int WINAPI VCI_Receive(unsigned int DevType,unsigned int DevIndex,unsigned int CANIndex,PVCI_CAN_OBJ pReceive,unsigned int Len,int WaitTime=-1);


#ifdef __cplusplus
}
#endif

#endif

