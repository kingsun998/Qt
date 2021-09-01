#ifndef COMPROCESS_H
#define COMPROCESS_H
#include <qobject.h>
#include <qthread.h>
#include <QTimer>
#include <windows.h>
#include <QVariant>
#include <qvector.h>

typedef struct{
    QWidget *widget;
    uint datalength;
    uint dequein;
    uint dequeout;
private:
    QString dcbinitstr=nullptr;
public:
    void initDcbstr(UINT baud, char parity,UINT databits, UINT stopsbits){
        dcbinitstr=QString("baud=%1 parity=%2 data=%3 stop=%4").arg(baud).arg(parity).arg(databits).arg(stopsbits);
    }
     QString getInitstr() const{
        return dcbinitstr;
    }
}ComParameter;


class ComService: public QObject
{
    Q_OBJECT
public:
    ComService();
    ~ComService();
signals:
    void receiveFromCom(QVariant var);
    void sendCallBack(QString mes);  //sendMessage的回调函数
public slots:
    bool init();
    bool sendMessage(QString mes);
    bool openPort();
    bool closePort();
    void setParameter(const ComParameter& parameter);
    void handleTimeOut();
public:
    void stopTimer();
    void startTimer();
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
    QTimer timer;
    QVector<unsigned char> frame;
    bool status;
};


class ComProcess : public QObject
{
    Q_OBJECT
public:
    explicit ComProcess(QObject *parent);
    ~ComProcess();
signals:

public slots:


public:
    void init(ComParameter& parameter);
    ComService* getService();
    void startServiceTimer();
    void stopServiceTimer();
private:
    QThread comthread;
    class ComService service;
};

#endif // COMPROCESS_H
