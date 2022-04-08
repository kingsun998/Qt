#include "comservice.h"
#include "QMessageBox"
#include <QDebug>
#include <qstringlist.h>
#include <settings.h>
#include <qstring.h>
#include <QVariant>
ComService::ComService(QObject *parent):QObject(parent){

}

ComService::~ComService(){

}

//void ComService::setParameter(const ComParameter parameter){
//    this->parent=parameter.widget;
//    this->dequein=parameter.dequein;
//    this->dequeout=parameter.dequeout;
//    this->Datalength=parameter.datalength;
//    CommTimeouts.ReadIntervalTimeout = parameter.ReadIntervalTimeout;
//    CommTimeouts.ReadTotalTimeoutMultiplier = parameter.ReadTotalTimeoutMultiplier;
//    CommTimeouts.ReadTotalTimeoutConstant = parameter.ReadTotalTimeoutConstant;
//    CommTimeouts.WriteTotalTimeoutMultiplier = parameter.WriteTotalTimeoutMultiplier;
//    CommTimeouts.WriteTotalTimeoutConstant = parameter.WriteTotalTimeoutConstant;
//    this->parity=parameter.parity;
//    this->baudRate=parameter.baudrate;
//    this->bitssize=parameter.bitssize;
//    this->stopbits=parameter.stopbits;
//    this->Comindex=parameter.comindex;
//    //初始化
//    this->init();
//}

bool ComService::openPort(const char* portname, int baudrate, char parity, char databit, char stopbit, char synchronizeflag)
{
    comhandle=INVALID_HANDLE_VALUE;
    //初始化临界区
    InitializeCriticalSection(&communicationSync);

    EnterCriticalSection(&communicationSync);

    /** 设备名,COM1,COM2等
     GENERIC_READ | GENERIC_WRITE, 访问模式,可同时读写
     0,                             共享模式,0表示不共享
     NULL,                         安全性设置,一般使用NULL
     OPEN_EXISTING,                该参数表示设备必须存在,否则创建失败
     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED   表示异步通信，若为0表示同步通信
     NULL //对串口而言该参数必须置为NULL
    **/


    comhandle=CreateFileA(this->Comindex.toLatin1().data(),GENERIC_READ|GENERIC_WRITE,0,NULL,
                          OPEN_EXISTING,0,NULL);
    /** 如果打开失败，释放资源并返回 */
    if (comhandle == INVALID_HANDLE_VALUE)
    {
        qDebug()<<"打开串口 Com4 失败";
        CloseHandle(comhandle);
        LeaveCriticalSection(&communicationSync);
        return false;
    }
    qDebug()<<"打开串口 COM4 成功";
    LeaveCriticalSection(&communicationSync);
    return true;
}

bool ComService::closePort(){
    /** 如果有串口被打开，关闭它 */
    if (comhandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(comhandle);
        comhandle = INVALID_HANDLE_VALUE;
    }
}

bool ComService::init(){
//    qDebug()<<"current thread id:"<<QThread::currentThreadId();
    qDebug()<<"串口 init";
//    if(!openPort()){
//        QMessageBox::information(parent,"错误","串口打开失败",QMessageBox::Warning);
//        return false;
//    }
    EnterCriticalSection(&communicationSync);
//    设置缓冲区
    SetupComm(comhandle,1024,1024);

    DCB  dcb;

    /** 获取当前串口配置参数,并且构造串口DCB参数 */

    if(!GetCommState(comhandle, &dcb)){
        qDebug()<<"获取状态失败";
        return false;
    }
    dcb.BaudRate=this->baudRate;
    dcb.ByteSize=this->bitssize;
    dcb.StopBits=this->stopbits;
    dcb.Parity=this->parity;

    /** 开启RTS flow控制 */
    //硬件流控制。数据终端就绪
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    if(!SetCommState(comhandle,&dcb)){
        qDebug()<<"设置状态失败";
        return false;
    }


    if(!SetCommTimeouts(comhandle, &CommTimeouts)){
        qDebug()<<"设置共同时间失败";
        return false;
    }
    /***
     * 设置串口的超时时间,均设为0,表示不使用超时限制
     * readintervaltimeout为每个字节之间的超时间隔
     * readtotaltimeoutMultiplier为所有字节总共发送的间隔
    ***/

//    清空缓冲区,包括写缓冲区，和读缓冲区。注意，清空不是清除
    PurgeComm(comhandle,PURGE_TXABORT|PURGE_TXCLEAR);
    PurgeComm(comhandle,PURGE_RXABORT|PURGE_RXCLEAR);
    LeaveCriticalSection(&communicationSync);
    status=false;
    qDebug()<<"初始化成功";
    return true;
}


ulong ComService::sendRequest(const unsigned char *array,uint arraysize,int arrayType)
{
    EnterCriticalSection(&communicationSync);
    //发送请求
    qDebug()<<"数组类型： "<<arrayType;
    ulong written=0;
    if(!WriteFile(comhandle,array,sizeof (unsigned char)*arraysize,&written,nullptr)){
        /** 获取错误码,可以根据该错误码查出错误原因 */
        DWORD dwError = GetLastError();
        qDebug()<<"发送请求错误，错误代码："<<dwError;
        /** 清空串口缓冲区 */
        PurgeComm(&communicationSync, PURGE_TXABORT | PURGE_TXCLEAR);
        return 0;
    }
    qDebug()<<"写入了"<<written<<"字节";
    ulong receiveAryLen=0;
    switch (arrayType) {
        //数据位+起始符+结尾符+校验和
        case 0:receiveAryLen=20;break;
        case 1:receiveAryLen=64;break;
        case 2:receiveAryLen=104;break;
        case 3:receiveAryLen=144;break;
        case 4:receiveAryLen=164;break;
    }
    qDebug()<<"预计接受: "<<receiveAryLen;
    unsigned char receive[2048];
    ulong BytesRead=0;
    bool status=ReadFile(comhandle, receive, receiveAryLen*2, &BytesRead, NULL);
    if (!status)
    {
        /** 获取错误码,可以根据该错误码查出错误原因 */
        DWORD dwError = GetLastError();
        qDebug()<<"接受错误，错误代码："<<dwError;
        /** 清空串口缓冲区 */
        PurgeComm(&communicationSync, PURGE_RXCLEAR | PURGE_RXABORT);
    }
    qDebug()<<"接收到"<<BytesRead<<"字节";
    QVector<unsigned char> vec;
    for(ulong i=0;i<BytesRead;i++){
        vec.push_back(receive[i]);
    }
    PurgeComm(comhandle,PURGE_RXABORT|PURGE_RXCLEAR);
    QVariant var;
    var.setValue(vec);
    emit callback(var,arrayType);
    LeaveCriticalSection(&communicationSync);
    return written;

}

ulong ComService::sendParameter(const unsigned char *array,uint arraysize,int arrayType){
    if(comhandle==INVALID_HANDLE_VALUE){
        QMessageBox::information(parent,"错误","初始化串口失败",QMessageBox::Warning);
        return 0;
    }
    qDebug()<<"service："<<QThread::currentThreadId();
    ulong written=0;
    EnterCriticalSection(&communicationSync);

    if(!WriteFile(comhandle,array,sizeof (unsigned char)*arraysize,&written,nullptr)){
        /** 获取错误码,可以根据该错误码查出错误原因 */
        DWORD dwError = GetLastError();
        qDebug()<<"发送错误，错误代码："<<dwError;
        /** 清空串口缓冲区 */
        PurgeComm(&communicationSync, PURGE_TXABORT | PURGE_TXCLEAR);
        return 0;
    }
    qDebug()<<"发送成功,写入大小:"<<written;
    LeaveCriticalSection(&communicationSync);
    return written;
}
