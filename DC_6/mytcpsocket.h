#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <mutex>

class MyTcpSocket : public QObject
{
Q_OBJECT
public:
    MyTcpSocket(QTcpSocket * socket,int seconds=10,QObject *parent = nullptr);
    ~MyTcpSocket();

signals:

private slots:
    void handleTimeOut();
    void resetTime(qint64 len);
private:
    int seconds;
    std::mutex m;
    QTcpSocket *socket;
    QTimer timer;

};

#endif // MYTCPSOCKET_H
