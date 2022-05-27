#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QTcpSocket * socket,int seconds,QObject *parent)
{
    this->seconds=seconds;
    timer.setInterval(1000);
    socket=socket;
    connect(&timer,&QTimer::timeout,this,&MyTcpSocket::handleTimeOut);
    connect(socket,&QTcpSocket::bytesWritten,this,&MyTcpSocket::resetTime);
    timer.start();
}

MyTcpSocket::~MyTcpSocket(){

}

void MyTcpSocket::handleTimeOut(){
    qDebug()<<seconds;
    std::lock_guard<std::mutex> lock(m);
    seconds-=1;
    if(seconds==0){
        socket->disconnectFromHost();
        qDebug()<<"delete this socket";
        delete socket;
        delete this;
    }
}

void MyTcpSocket::resetTime(qint64 len){
    qDebug()<<"Written "<<len<<" bytes";
    this->seconds=10;
}
