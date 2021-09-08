#ifndef COMPROCESS_H
#define COMPROCESS_H
#include <qobject.h>
#include <qthread.h>
#include <QTimer>
#include <windows.h>
#include <QVariant>
#include <qvector.h>
#include <comservice.h>
#include <subcomdialog.h>
#define COMMAND 11

extern class ComProcess comprocess;
//相当于一个中间层
class ComProcess : public QObject
{
    Q_OBJECT
public:
     ComProcess(QObject *parent=nullptr);
    ~ComProcess();
signals:
    void sendRequestCommand(const unsigned char *array,uint arraysize,int arrayType);
    void sendParameterCommand(const unsigned char *array,uint arraysize,int arrayType);
    void returnResult(QVariant var,int arraytype);

    void setParameterCommand(ComParameter comparameter);
public slots:
    void handleTimeOut();
    void getResult(QVariant var,int arytype);
public:
    void init(ComParameter& parameter);
    ComService* getService();
    //开启请求计时器
    void startComProcessTimer();
    //关闭请求计时器
    void stopComProcessTimer();
    //发送参数给COM
    void sendComParameter(Message& mes);
private:
    QThread comthread;
    ComService service;

    QTimer timer;
    unsigned char D20_D23[11];
    unsigned char D380_D393[11];
    unsigned char D422_D446[11];
    unsigned char D690_D725[11];
    unsigned char D1123_D1160[11];
    bool ifsending;
};

#endif // COMPROCESS_H
