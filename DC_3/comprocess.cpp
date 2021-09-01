#include "comprocess.h"
#include "QMessageBox"
#include "process.h"
#include <QDebug>
#include <qstringlist.h>

ComProcess::ComProcess(QObject *parent) : QObject(parent)
{
}

ComProcess::~ComProcess(){
}

void ComProcess::init(ComParameter& comparameter){
    service.setParameter(comparameter);
    service.init();
}

void ComProcess::startServiceTimer(){
    service.startTimer();
}

void ComProcess::stopServiceTimer(){
    service.stopTimer();
}

ComService* ComProcess::getService(){
    return &service;
}




ComService::ComService(){
}

ComService::~ComService(){

}
void ComService::startTimer(){
    timer.start();
    qDebug()<<"timer start";
}
void ComService::stopTimer(){
    timer.stop();
    qDebug()<<"timer stop";
}
void ComService::setParameter(const ComParameter& parameter){
    this->parent=parameter.widget;
    this->dequein=parameter.dequein;
    this->dequeout=parameter.dequeout;
    this->Datalength=parameter.datalength;
    if(parameter.getInitstr() ==nullptr){
        QMessageBox::information(parent,"错误","请先初始化结构体的DCB",QMessageBox::Warning);
        return;
    }
    this->initstr=parameter.getInitstr();
}

bool ComService::openPort(){
    comhandle=INVALID_HANDLE_VALUE;
    //初始化临界区
    InitializeCriticalSection(&communicationSync);
    timer.setInterval(50);
    connect(&timer,&QTimer::timeout,this,&ComService::handleTimeOut);

    EnterCriticalSection(&communicationSync);

    /** 设备名,COM1,COM2等
     GENERIC_READ | GENERIC_WRITE, 访问模式,可同时读写
     0,                             共享模式,0表示不共享
     NULL,                         安全性设置,一般使用NULL
     OPEN_EXISTING,                该参数表示设备必须存在,否则创建失败
    **/
    comhandle=CreateFileA("COM4",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
    /** 如果打开失败，释放资源并返回 */
    if (comhandle == INVALID_HANDLE_VALUE)
    {
        LeaveCriticalSection(&communicationSync);
        return false;
    }
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
    qDebug()<<"串口 init";
    if(!openPort()){
        QMessageBox::information(parent,"错误","串口打开失败",QMessageBox::Warning);
        return false;
    }
    EnterCriticalSection(&communicationSync);
//    设置缓冲区
    SetupComm(comhandle,2*dequein,2*dequeout);
    /***
     * 设置串口的超时时间,均设为0,表示不使用超时限制
     * readintervaltimeout为每个字节之间的超时间隔
     * readtotaltimeoutMultiplier为所有字节总共发送的间隔
    ***/
    CommTimeouts.ReadIntervalTimeout = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = 0;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = 0;
//  设置缓冲
    if(!SetCommTimeouts(comhandle, &CommTimeouts)){
        qDebug()<<"设置共同时间失败";
        return false;
    }
    DCB  dcb;
    /** 获取当前串口配置参数,并且构造串口DCB参数 */
    //c++计算是从右向左计算的，所以先进行
    //buildcommDcb,通过字符串来初始化dcb
    qDebug()<<initstr;
    if(!BuildCommDCBA(initstr.toStdString().c_str(),&dcb)){
        qDebug()<<"创建DCB 失败";
        return false;
    }
    if(!GetCommState(comhandle, &dcb)){
        qDebug()<<"获取状态失败";
        return false;
    }
    /** 开启RTS flow控制 */
    //硬件流控制。数据终端就绪
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    if(!SetCommState(comhandle,&dcb)){
        qDebug()<<"设置状态失败";
        return false;
    }

//    清空缓冲区,包括写缓冲区，和读缓冲区。注意，清空不是清除
    PurgeComm(comhandle,PURGE_TXABORT|PURGE_TXCLEAR);
    PurgeComm(comhandle,PURGE_RXABORT|PURGE_RXCLEAR);
    LeaveCriticalSection(&communicationSync);
    status=false;
    return true;
}

Q_DECLARE_METATYPE(QVector<unsigned char>)
void ComService::handleTimeOut(){
    EnterCriticalSection(&communicationSync);
    /** 从缓冲区读取一个字节的数据 */
    //读取字节数的缓冲区
    unsigned char receive[dequein*2];
    //实际读取的字节数
    ulong BytesRead;
    //ReadFile可以从文件中读取内容，同时，也可以从串口中读取数据
    //读的datalength长度
    if (!ReadFile(comhandle, receive, Datalength, &BytesRead, NULL))
    {
        /** 获取错误码,可以根据该错误码查出错误原因 */
        DWORD dwError = GetLastError();
        qDebug()<<"错误代码："<<dwError;
        /** 清空串口缓冲区 */
        PurgeComm(&communicationSync, PURGE_RXCLEAR | PURGE_RXABORT);
    }
    for (int i=0;i<BytesRead;i++) {
        if(receive[i]==0xff){
            status=true;
        }
        else if(receive[i]==0xfe){
            status=false;
            QVector<unsigned char> newframe;
            newframe.swap(frame);
            QVariant variant;
            variant.setValue(newframe);
            emit receiveFromCom(variant);
        }else{
            frame.push_back(receive[i]);
        }
    }
    LeaveCriticalSection(&communicationSync);
}

bool ComService::sendMessage(QString mes){
    QStringList sl= mes.split(" ");
    QVector<uint> m;
    int len=sl.size();
    m.push_back(0xff);
    for(int i=0;i<len;i++){
        if(sl[i]!=" "){
            m.push_back(sl[i].toUInt(nullptr,16));
        }
    }
    m.push_back(0xfe);
    unsigned char sendmes[m.size()];
    ulong written=0;
    if(!WriteFile(comhandle,&m,m.size(),&written,nullptr)){
        /** 获取错误码,可以根据该错误码查出错误原因 */
        DWORD dwError = GetLastError();
        qDebug()<<"错误代码："<<dwError;
        /** 清空串口缓冲区 */
        PurgeComm(&communicationSync, PURGE_TXABORT | PURGE_TXCLEAR);
        return false;
    }
    emit sendCallBack(mes);

}
