#include "httpserver.h"
//#include "ui_qt_httpserver.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QFile>

HttpServer &HttpServer::instance()
{
    static HttpServer obj;
    return obj;
}


HttpServer::HttpServer(QObject *parent) : QObject(parent)
{
    m_httpServer = new QTcpServer(this);
    m_httpServer->setMaxPendingConnections(3);//设置最大允许连接数
    QObject::connect(m_httpServer,&QTcpServer::newConnection,this,&HttpServer::newConnection);
}

HttpServer::~HttpServer()
{

}


void HttpServer::run(const QHostAddress &address, const quint16 &port)
{

    m_httpServer->listen(address,port);
}

void HttpServer::newConnection()
{
    qDebug() << "newConnection";
    num = rand()%4;
    //获取下一个可用的连接
    QTcpSocket *m_socket = m_httpServer->nextPendingConnection();
//    socket_list.append(new MyTcpSocket(m_socket,10));

    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&HttpServer::readyRead);
}

void HttpServer::readyRead()
{
//    当某一个Object emit一个signal的时候，它就是一个sender,系统会记录下当前是谁emit出这个signal的，
//    所以你在对应的slot里就可以通过 sender()得到当前是谁invoke了你的slot，对应的是QObject->d->sender.
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
//  存在是由 QTcpSocket 发送的信号

    //套接字需要关闭后才能将东西真正输出到屏幕
    if(socket){
        QByteArray request = socket->readAll();

        qDebug() << "Request Data:" << request;

        static int count = 0;
        count++;
        QByteArray a = QString("<h1><center>快去学习 %1</center></h1>\r\n").arg(count).toUtf8();
        QByteArray b = QString("<h1><center>别玩了 %1</center></h1>\r\n").arg(count).toUtf8();
        QByteArray c = QString("<h1><center>别玩手机了 %1</center></h1>\r\n").arg(count).toUtf8();
        QByteArray z = QString("<h1><center>倒霉蛋 %1</center></h1>\r\n").arg(count).toUtf8();
        QByteArray response;

        switch (num) {
        case 1:
            response = a;
            break;
        case 2:
            response = b;
            break;
        case 3:
            response = c;
            break;
        default:
            response = z;

        }

        //    qint64 write(const char *data);
                /*
        # 以二进制形式打开图片
        f = open(filename,'rb')
        # 响应头
        responseHeaders="HTTP/1.1 200 OK\r\n"
        responseHeaders += '\r\n'
        # 响应体
        responseBody = f.read()
        # 整合，响应头编码为二进制
        response = responseHeaders.encode() + responseBody
        # 发送
        connfd.send(response)
        */

        //图片
//        QFile file("C:\\Users\\Allen\\Pictures\\pictrue\\1.png");
//        file.open(QIODevice::ReadOnly);
//        response = file.read(2079181);
        QString http  = "HTTP/1.1 200 OK\r\n";
        http  += "\r\n";

        // 文字
        /*
        QString http = "HTTP/1.1 200 OK\r\n";
        http += "Server: nginx\r\n";
        http += "Content-Type: text/html;charset=utf-8\r\n";
        http += "Connection: keep-alive\r\n";
        http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(response.size()));
        */

        socket->write(http.toUtf8());
        socket->write(response);
        qDebug()<<response;
        socket->flush();
        //最多等待3s钟
        socket->waitForBytesWritten(3000);
        socket->close();
    }
}
