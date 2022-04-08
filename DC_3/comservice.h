#ifndef COMSERVICE_H
#define COMSERVICE_H
#include <qobject.h>
#include <qthread.h>
#include <windows.h>
#include <QVariant>
#include <subcomdialog.h>
typedef struct{
    QWidget *widget;
    uint datalength;
    uint dequein;
    uint dequeout;
    //读取间隔
    ulong ReadIntervalTimeout;
    ulong ReadTotalTimeoutMultiplier;
    ulong ReadTotalTimeoutConstant;
    ulong WriteTotalTimeoutMultiplier;
    ulong WriteTotalTimeoutConstant;

    QString comindex;
    //奇偶校验位
    unsigned char parity;
    //波特率
    long baudrate;
    unsigned char stopbits;
    unsigned char bitssize;

}ComParameter;

class ComService: public QObject
{
    Q_OBJECT
public:
    ComService(QObject *parent=nullptr);
    ~ComService();
signals:
    void callback(QVariant var,int arraytype);  //sendRequest的回调函数
    void sendFinish();    //sendParameter的回调函数
public slots:
    bool init();
    bool openPort(const char* portname, int baudrate=115200, char parity=0, char databit=8, char stopbit=1, char synchronizeflag=0);
    bool closePort();
    //用于发送请求，
    ulong sendRequest(const unsigned char *array,uint arraysize,int arrayType);
    ulong sendParameter(const unsigned char *array,uint arraysize,int arrayType);
private:
    //串口句柄和临界区标志
    HANDLE comhandle;
    CRITICAL_SECTION   communicationSync;
    //父节点，需要时qwidget类型
    QWidget *parent;
    //数组长度
    uint Datalength;
    //dcb初始化字符串
    QString initstr;
    //输入输出缓冲区的大小
    uint dequein;
    uint dequeout;
    //串口的超时设定
    COMMTIMEOUTS  CommTimeouts;
    bool status;

    //串口结构体设置
    long baudRate;
    unsigned char stopbits;
    unsigned char bitssize;
    unsigned char parity;

    QString Comindex;
};
#endif // COMSERVICE_H
