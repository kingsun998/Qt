#include "utils.h"

mode_base::mode_base(QWidget *parent):
    QDialog(parent)
{
    this->setHidden(true);
}
mode_base::~mode_base(){

}

MqttDataStruct::MqttDataStruct(){

}

void MqttDataStruct::AddObject(QJsonObject &obj){
    ary.append(obj);
}

QByteArray MqttDataStruct::tojson(){
    return QJsonDocument(ary).toJson(QJsonDocument::Compact);
}

MqttDataStruct::~MqttDataStruct(){

}
