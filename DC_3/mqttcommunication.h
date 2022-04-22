#ifndef MQTTCOMMUNICATION_H
#define MQTTCOMMUNICATION_H

#include <QObject>
#include <QtMqtt/qmqttclient.h>
#include <mqttdatastruct.h>
extern class MqttCommunication mqtt;
class MqttCommunication:public QObject
{
    Q_OBJECT
public:
    MqttCommunication();
    ~MqttCommunication();
    void publish_Mes(MqttDataStruct& mes);
    void init(QString ip_address,QString port,QString topic);
    void connectToHost();
    void scribe();


public slots:
    void connectSuccess();
    void updateLogStateChange();
private:
    QMqttClient *m_client;
    QString ip_address;
    quint16 port;
    QString topic;
    quint8 qos;
    QMqttSubscription* subscription;
};

#endif // MQTTCOMMUNICATION_H
