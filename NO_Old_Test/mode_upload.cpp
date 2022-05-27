#include "mode_upload.h"
#include "ui_mode_upload.h"
#include <settings.h>
#include <QMessageBox>
#include <utils.h>
mode_upload::mode_upload(QWidget *parent) :
    mode_base (parent),
    ui(new Ui::mode_upload)
{
    ui->setupUi(this);
    m_client=new QMqttClient(nullptr);
    connect(m_client,  &QMqttClient::stateChanged,
            this,&mode_upload::updateLogStateChange);
}

void mode_upload::reset_config(QString ip,QString port,QString topic,int qos)
{
    ip_address=ip;
    this->port=port.toUInt();
    this->topic=topic;
    this->qos=qos;
    m_client->disconnectFromHost();
    m_client->setHostname(ip);
    m_client->setPort(port.toUInt());
    m_client->setUsername(settings.get("mqtt","username").toString());
    m_client->setPassword(settings.get("mqtt","password").toByteArray());
    connectToHost();
    scribe();

}

void mode_upload::connectToHost(){
    m_client->connectToHost();
}
void mode_upload::scribe(){
    subscription = m_client->subscribe(topic,qos);
}

void mode_upload::updateLogStateChange(){
    if(m_client->state()==2)
        QMessageBox::information(nullptr,"提示","连接成功",QMessageBox::Warning);
}

void mode_upload::publish_Mes(MqttDataStruct& data){

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

mode_upload::~mode_upload()
{
    delete ui;
}

void mode_upload::reset(){

}
void mode_upload::init(){
    QString ip=settings.get("mqtt","mqtt_ip_address").toString();
    QString port=settings.get("mqtt","port").toString();
    QString topic=settings.get("mqtt","topic").toString();
    int qos= settings.get("mqtt","qos").toUInt();
    reset_config(ip,port,topic,qos);
}


void mode_upload::on_pushButton_clicked()
{
    MqttDataStruct data;
    publish_Mes(data);
}
