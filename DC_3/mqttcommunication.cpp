#include "mqttcommunication.h"
#include "settings.h"
#include <QMessageBox>
#include <qdatetime.h>
class MqttCommunication mqtt;
MqttCommunication::MqttCommunication()
{
    m_client=new QMqttClient(nullptr);

    qos=settings.qos;
    m_client->setHostname(settings.mqtt_ip_address);
    m_client->setPort(settings.mqtt_port);
    this->topic=settings.topic;
}

MqttCommunication::~MqttCommunication(){

}
void MqttCommunication::init(QString ip,QString port,QString topic)
{
    qos=settings.qos;
    m_client->setHostname(ip);
    m_client->setPort(port.toUInt());
    this->topic=topic;
    m_client->connectToHost();
    qDebug()<<"mqtt connected state:"<<m_client->Connected;
}

void MqttCommunication::connect(){
    m_client->connectToHost();
}

void MqttCommunication::scribe(){
    subscription = m_client->subscribe(topic,qos);
}

void MqttCommunication::connectSuccess(){
    QMessageBox::information(nullptr, QLatin1String("Success"),
                          "连接成功");
}

void MqttCommunication::publish_Mes(MqttDataStruct& data){

    qDebug()<<m_client->state();
    qDebug()<<QMqttClient::Connected;
    qDebug()<<"subscription："<<subscription;
    if(subscription){
        QByteArray&& ary=data.tojson();
        if(m_client->publish(topic,ary,qos)== -1){

            QMessageBox::critical(nullptr, QLatin1String("Error"),
                                  "发送失败");
        }else{
            QMessageBox::information(nullptr, QLatin1String("Success"),
                                  "发送成功");
        }
    }else{
        QMessageBox::critical(nullptr, QLatin1String("Error"),
                              "订阅失败");
    }
}

