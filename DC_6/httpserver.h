#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QtNetwork>
#include <qvector.h>
#include <mytcpsocket.h>
#include <scangun.h>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    static HttpServer &instance();
    void run(const QHostAddress &address = QHostAddress::Any,const quint16 &port = 80);
signals:

public slots:
private slots:
    void newConnection();
    void readyRead();
private:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer();

private:
    QTcpServer *m_httpServer;
    int num;


};


#endif // HTTPSERVER_H
