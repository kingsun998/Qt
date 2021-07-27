/*******************************************************************************
  * @file    LDFDecoder.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   LDF�ļ�������غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __LDFDECODER_H_
#define __LDFDECODER_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

#define LDF_SUCCESS                 0   //�����ɹ�
#define LDF_ERROR_FILE_OPEN         (-1)//���ļ�����
#define LDF_ERROR_FILE_FORMAT       (-2)//�ļ���ʽ����
#define LDF_ERROR_DEV_DISCONNECT    (-3)//�豸δ����

//�߼����ͱ���ֵ
typedef struct _LDF_CODING_LOGICAL{
    int RawValue;       //ʵ��ֵ
    char *pDescription; //�߼�ֵ�����ַ���
}LDF_CODING_LOGICAL;

//�������ͱ���ֵ���ź�����ֵ=�ź�ʵ��ֵ*Factor+Offset
typedef struct _LDF_CODING_PHYSICAL{
    float Offset;   //�ź�����ֵƫ����
    float Factor;   //�ź���������
    int Minimum;    //�ź�ʵ����Сֵ��ע�ⲻ������ֵ
    int Maximum;    //�ź�ʵ�����ֵ��ע�ⲻ������ֵ
    char *pUnit;    //�ź�����ֵ��λ
}LDF_CODING_PHYSICAL;

//�ź��е�ֵ����
enum LDF_SIGNAL_VALUE_TYPE{
    LDF_SIGNAL_VALUE_TYPE_SCALAR,   //����ֵ
    LDF_SIGNAL_VALUE_TYPE_ARRAY     //����ֵ
};

//�ź�ֵ��������
enum LDF_SIGNAL_CODING_TYPE{
    LDF_SIGNAL_CODING_TYPE_NONE,
    LDF_SIGNAL_CODING_TYPE_PHYSICAL,//����ֵ
    LDF_SIGNAL_CODING_TYPE_LOGICAL  //�߼�ֵ
};

//�ź�����
enum LDF_SIGNAL_TYPE{
    LDF_SIGNAL_TYPE_NORMAL,
    LDF_SIGNAL_TYPE_DIAGNOSTIC
};

//֡����
enum LDF_FRAME_TYPE{
    LDF_FRAME_TYPE_NORMAL,
    LDF_FRAME_TYPE_DIAGNOSTIC
};

//�źű���
typedef struct _LDF_SIGNAL_CODING{
    char *pName;                            //�źű�������
    int LogicalValueLen;                    //�߼�ֵ����
    int PhysicalValueLen;                   //����ֵ����
    LDF_CODING_LOGICAL *pLogicalValues;     //�߼�ֵ�б�ָ��
    LDF_CODING_PHYSICAL *pPhysicalValues;   //����ֵ�б�ָ��
}LDF_SIGNAL_CODING;

//�źź��źű����Ӧֵ
typedef struct _LDF_SIGNAL_REPRESENTATION{
    char *pSignalName;  //�ź�����
    char *pCodingName;  //��������
}LDF_SIGNAL_REPRESENTATION;

//�ź�ֵ�ṹ�壬�ṹ�������ֵ��ȡһ��������ͬʱ����
typedef struct _LDF_SIGNAL_VALUE{
    long long ScalarValue;      //����ֵ
    unsigned char ArrayValue[8];//����
}LDF_SIGNAL_VALUE;

//�źű���
typedef struct _LDF_LIN_SIGNAL{
    char *pName;                    //�ź�����
    int Size;                       //�źŴ�С����λΪbit
    LDF_SIGNAL_TYPE Type;           //��ͨ�źŻ�������ź�
    LDF_SIGNAL_VALUE_TYPE ValueType;//��ֵ��������
    LDF_SIGNAL_VALUE Value;         //�ź�ֵ
    char *pPublisher;               //�źŷ��������ƣ�����Ϊ�������ߴӻ�
    int SubscriberLen;              //���ո��źŽڵ㳤��
    char *pSubscribers[16];         //���ո��źŵĽڵ���������
    int Offset;                     //���ź���8�ֽ������е�ƫ����
    LDF_SIGNAL_CODING SignalCodings;//�źű���
}LDF_LIN_SIGNAL;

//֡��ÿ֡���ݰ�������ź�
typedef struct _LDF_LIN_FRAME{
    char *pName;                //֡����
    unsigned char ID;           //֡ID
    char *pPublisher;           //����������
    unsigned char Size;         //֡���ȣ���λΪ�ֽ�
    LDF_FRAME_TYPE Type;        //֡����
    int SignalListLen;          //�źų���
    LDF_LIN_SIGNAL *pSignalList;//�ź�ָ��
}LDF_LIN_FRAME;
//�ڵ�ṹ��
typedef struct _LDF_LIN_NODE{
    char *pMasterName;      //��������
    float time_base;
    float jitter;
    int SlaveLen;           //�ӻ�����
    char *pSlaveNames[16];  //�ӻ���������
}LDF_LIN_NODE;

//���ȱ�����
typedef struct _LDF_LIN_SCH_CMD{
    LDF_LIN_FRAME Frame;    //LIN֡
    float DelayOfMs;        //��ʱʱ�䣬��λΪ����
}LDF_LIN_SCH_CMD;

//���ȱ�
typedef struct _LDF_LIN_SCH{
    char *pName;                        //���ȱ�����
    int ScheduleCmdLen;                 //�ñ����������֡��
    LDF_LIN_SCH_CMD *pScheduleCmdList;  //�ı������֡�б�
}LDF_LIN_SCH;

//�ڵ�����
typedef struct _LDF_NODE_ATTR{
    char *pName;
    unsigned short LIN_protocol;
    unsigned char configured_NAD;
    unsigned char initial_NAD;
    struct {
        unsigned int supplier_id;
        unsigned int function_id;
        unsigned int variant;
    }product_id;
    char *response_error;//signal_name
    char *fault_state_signals[64];
    int fault_state_signal_len;
    float P2_min;
    float ST_min;
    float N_As_timeout;
    float N_Cr_timeout;
    char *configurable_frames[64];
    int configurable_frame_len;
}LDF_NODE_ATTR;

//�������LDF�ļ�����
typedef struct _LDF_CODING{
    int ProtocolVersion;        //LIN protocol version,ʵ�ʰ汾�ŵ�һ�ٱ�ֵ�����統ǰֵΪ210����ôʵ�ʰ汾Ϊ2.1
    int LanguageVersion;        //LIN language version,ʵ�ʰ汾�ŵ�һ�ٱ�ֵ�����統ǰֵΪ210����ôʵ�ʰ汾Ϊ2.1
    int LINSpeed;               //LIN���߲����ʣ���λΪbps
    LDF_LIN_NODE *pNode;        //�ڵ�ָ��
    int FrameLen;               //֡����
    LDF_LIN_FRAME *pFrames;     //ָ֡��
    int NodeAttrLen;            //�ڵ����Գ���
    LDF_NODE_ATTR *pNodeAttrs;  //�ڵ�����ָ��
    int LINSchLen;              //���ȱ���
    LDF_LIN_SCH *pLINSch;       //���ȱ�ָ��
}LDF_CODING;

#ifdef __cplusplus
extern "C"
{
#endif
    int WINAPI LDF_Decode(int DevHandle,char *pLDFFileName,LDF_CODING *pLDFCoding);
    int WINAPI LDF_DataRawToFrame(LDF_LIN_FRAME *pFrame,unsigned char *pRawData);
    int WINAPI LDF_DataFrameToRaw(LDF_LIN_FRAME *pFrame,unsigned char *pRawData);
#ifdef __cplusplus
}
#endif

#endif


