/**
  ******************************************************************************
  * @file    usb2lin_ex.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2lin_ex��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2LIN_EX_H_
#define __USB2LIN_EX_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif
//ע�⣺ʹ����Щ������Ҫ1.5.30�����ϵĹ̼���֧��
//���庯�����ش������
#define LIN_EX_SUCCESS             (0)   //����ִ�гɹ�
#define LIN_EX_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define LIN_EX_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define LIN_EX_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define LIN_EX_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
#define LIN_EX_ERR_CH_NO_INIT      (-5)  //��ͨ��δ��ʼ��
#define LIN_EX_ERR_READ_DATA       (-6)  //LIN������ʧ��
#define LIN_EX_ERR_PARAMETER       (-7)  //����������������
//У������
#define LIN_EX_CHECK_STD     0  //��׼У�飬����PID
#define LIN_EX_CHECK_EXT     1  //��ǿУ�飬����PID
#define LIN_EX_CHECK_USER    2  //�Զ���У�����ͣ���Ҫ�û��Լ����㲢����Check���������Զ�У��
#define LIN_EX_CHECK_NONE    3  //������У������
#define LIN_EX_CHECK_ERROR   4  //��������У�����
//���ӻ�����
#define LIN_EX_MASTER        1  //����
#define LIN_EX_SLAVE         0  //�ӻ�
//֡����
#define LIN_EX_MSG_TYPE_UN      0  //δ֪����
#define LIN_EX_MSG_TYPE_MW      1  //������ӻ���������
#define LIN_EX_MSG_TYPE_MR      2  //�����Ӵӻ���ȡ����
#define LIN_EX_MSG_TYPE_SW      3  //�ӻ���������
#define LIN_EX_MSG_TYPE_SR      4  //�ӻ���������
#define LIN_EX_MSG_TYPE_BK      5  //ֻ����BREAK�źţ����Ƿ������������ݣ�����ֻ��⵽BREAK�ź�
#define LIN_EX_MSG_TYPE_SY      6  //������⵽��BREAK��SYNC�ź�
#define LIN_EX_MSG_TYPE_ID      7  //������⵽��BREAK��SYNC��PID�ź�
#define LIN_EX_MSG_TYPE_DT      8  //������⵽��BREAK��SYNC��PID,DATA�ź�
#define LIN_EX_MSG_TYPE_CK      9  //������⵽��BREAK��SYNC��PID,DATA,CHECK�ź�

typedef struct _LIN_EX_MSG{
    unsigned int  Timestamp;    //�ӻ���������ʱ����ʱ�������λΪ0.1ms;������д����ʱ����ʾ���ݶ�д�����ʱʱ�䣬��λΪms
    unsigned char MsgType;      //֡����
    unsigned char CheckType;    //У������
    unsigned char DataLen;      //LIN���ݶ���Ч�����ֽ���
    unsigned char Sync;         //�̶�ֵ��0x55
    unsigned char PID;          //֡ID
    unsigned char Data[8];      //����
    unsigned char Check;        //У��,ֻ��У����������ΪLIN_EX_CHECK_USER��ʱ�����Ҫ�û���������
    unsigned char BreakBits;    //��֡��BRAK�ź�λ������ЧֵΪ10��26��������Ϊ����ֵ��Ĭ��Ϊ13λ
    unsigned char Reserve1;
}LIN_EX_MSG,*PLIN_EX_MSG;

#ifdef __cplusplus
extern "C"
{
#endif
//��ʼ��
int WINAPI  LIN_EX_Init(int DevHandle,unsigned char LINIndex,unsigned int BaudRate,unsigned char MasterMode);
//����ģʽ��������
int WINAPI  LIN_EX_MasterSync(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pInMsg,LIN_EX_MSG *pOutMsg,unsigned int MsgLen);
int WINAPI  LIN_EX_MasterWrite(int DevHandle,unsigned char LINIndex,unsigned char PID,unsigned char *pData,unsigned char DataLen,unsigned char CheckType);
int WINAPI  LIN_EX_MasterRead(int DevHandle,unsigned char LINIndex,unsigned char PID,unsigned char *pData);
//�ӻ�ģʽ��������
int WINAPI  LIN_EX_SlaveSetIDMode(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pLINMsg,unsigned int MsgLen);
int WINAPI  LIN_EX_SlaveGetIDMode(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pLINMsg);
int WINAPI  LIN_EX_SlaveGetData(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pLINMsg);
//��Դ������غ���
int WINAPI  LIN_EX_CtrlPowerOut(int DevHandle,unsigned char State);
int WINAPI  LIN_EX_GetVbatValue(int DevHandle,unsigned short *pBatValue);
//����ģʽ�Զ�����������غ���
int WINAPI  LIN_EX_MasterStartSch(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pLINMsg,unsigned int MsgLen);
int WINAPI  LIN_EX_MasterStopSch(int DevHandle,unsigned char LINIndex);
int WINAPI  LIN_EX_MasterGetSch(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pLINMsg);

int WINAPI  LIN_EX_DecodeListFile(char *pFileName,char CheckType,int BaudRate,char *pReadDataList,char ReadDataListLen,char *pCheckTypeList,char CheckTypeListLen);
int WINAPI  LIN_EX_GetListFileMsg(int MsgIndex,int MsgLen,LIN_EX_MSG *pLINMsg);

#ifdef __cplusplus
}
#endif
#endif

