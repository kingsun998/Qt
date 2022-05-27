#ifndef WZSERIALPORT_H
#define WZSERIALPORT_H

#include <QObject>
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <QThread>
#include <vector>
#include <string>
#include <QThread>
#include <QTimer>
#include <vector>
/*
 *
    作者：欧阳伟
    日期：2017-12-14
    类名：WZSerialPort
    用途：串口读写
    示例：
        WZSerialPort w;
        if(w.open("COM1",9600，0，8，1))
        {
            w.send("helloworld",10);
            char buf[1024];
            w.receive(buf,1024);
        }
     王阳 改写
*/
extern class WZSerialPort wzserialport_eight;
extern class WZSerialPort wzserialport_single;
extern class WZSerialPort wzserialport_multiple;
class PortStruct
{
public:
    int state;
    QString testtime;
    PortStruct();
    virtual ~PortStruct();
    virtual void reset();
};

class SinglePortStruct: public PortStruct
{
public:
    std::string voltage;
    std::string resistance;
    std::string electric;
    std::string testState;
    std::string time;
    SinglePortStruct();
    virtual ~SinglePortStruct();
    void reset();
};

class EightPortStruct: public PortStruct
{
public:
    std::vector<std::string> resistance;
    std::vector<std::string> voltage;
    std::vector<bool> testState;
    int time;
    EightPortStruct();
    virtual ~EightPortStruct();
    void reset();
};

class MultipleProcessStruct: public QObject,public PortStruct
{
    Q_OBJECT
public:
    std::string pcdID;
    double highTemperature;
    double lowTemperature;
    MultipleProcessStruct();
    virtual ~MultipleProcessStruct();
    void reset();
signals:
    void finish();
};

class WZSerialPort:public QObject
{
    Q_OBJECT
public:
    WZSerialPort(int state,QObject *parent=nullptr);
    ~WZSerialPort();

    // 打开串口,成功返回true，失败返回false
    // portname(串口名): 在Windows下是"COM1""COM2"等，在Linux下是"/dev/ttyS1"等
    // baudrate(波特率): 9600、19200、38400、43000、56000、57600、115200
    // parity(校验位): 0为无校验，1为奇校验，2为偶校验，3为标记校验
    // databit(数据位): 4-8，通常为8位
    // stopbit(停止位): 1为1位停止位，2为2位停止位,3为1.5位停止位
    // synchronizable(同步、异步): 0为异步，1为同步
    bool open(const char* portname, DWORD baudrate=2400, char parity=0, char databit=8, char stopbit=1, char synchronizeflag=0);

    //关闭串口，参数待定
    void close();

    std::vector<std::string> getComPort();

    QStringList get_available_list();

    void setPortStruct(PortStruct* pt);
private:
    //发送数据或写数据，成功返回发送数据长度，失败返回0
    DWORD send(byte* command,int size);
    //接受数据或读数据，成功返回读取实际数据的长度，失败返回0
    bool receive(void (Decode)(char*,int,bool,PortStruct*));

signals:
    void send_receive();
    void send_multiple_receive();
    void ReceiveSinglePortMes();
    void ReceiveEightPortMes();
    void listen_to_port_signal();
private slots:
    void send_receive_synchronously();
    void send_multiple_receive_synchronously();
    void listen_to_port();
private:
    int pHandle[16];
    char synchronizeflag;
    QThread thread;
    QTimer timer;
    bool connected;
    PortStruct *pt;

    int state;
};




#endif  // WZSERIALPORT_H
