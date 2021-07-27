/**
  ******************************************************************************
  * @file    lin_uds.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   lin_uds��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __LIN_UDS_H_
#define __LIN_UDS_H_

#include <stdint.h>
#include "offline_type.h"
#include "usb2lin_ex.h"
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

#define LIN_UDS_OK            0
#define LIN_UDS_TRAN_USB      -98
#define LIN_UDS_TRAN_LIN      -99
#define LIN_UDS_TIMEOUT_A     -100
#define LIN_UDS_TIMEOUT_Bs    -101
#define LIN_UDS_TIMEOUT_Cr    -102
#define LIN_UDS_WRONG_SN      -103
#define LIN_UDS_INVALID_FS    -104
#define LIN_UDS_UNEXP_PDU     -105
#define LIN_UDS_WFT_OVRN      -106
#define LIN_UDS_BUFFER_OVFLW  -107
#define LIN_UDS_ERROR         -108

//1.LIN UDS��ַ����
typedef  struct  _LIN_UDS_ADDR
{
    unsigned char   ReqID; //������ID��
    unsigned char   ResID; //Ӧ����ID��
    unsigned char   NAD;   //�ڵ��ַ��0x7FΪ�㲥��ַ
    unsigned char   CheckType;//0-��׼��1-��ǿ
    unsigned char   STmin;  //����֡ʱ��������λΪ����
}LIN_UDS_ADDR;

#ifdef __cplusplus
extern "C"
{
#endif

    int WINAPI LIN_UDS_Request(int DevHandle,unsigned char LINIndex,LIN_UDS_ADDR *pUDSAddr,unsigned char *pReqData,int DataLen);
    int WINAPI LIN_UDS_Response(int DevHandle,unsigned char LINIndex,LIN_UDS_ADDR *pUDSAddr,unsigned char *pResData,int TimeOutMs);
    int WINAPI LIN_UDS_GetMsgFromUDSBuffer(int DevHandle,unsigned char LINIndex,LIN_EX_MSG *pLINMsg,int BufferSize);

#ifdef __cplusplus
}
#endif
#endif


