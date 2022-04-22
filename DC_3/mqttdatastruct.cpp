#include "mqttdatastruct.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

MqttDataStruct::MqttDataStruct()
{

}
void MqttDataStruct::AddObject(QString name,QString properties,const QString val){
    if(map.find(name)==map.end()){
        QMap<QString,QString> sensor;
        map[name]=sensor;
    }
    map[name][properties]=val;
}

void MqttDataStruct::AddTestName(int code){
    this->testCode=code;
}

QByteArray MqttDataStruct::tojson(){
    QMap<QString,QMap<QString,QString>>::iterator itr=map.begin();
    QJsonObject obj;
    QJsonObject TestName;
    QJsonArray array;
    while (itr!=map.end()) {
        auto submp=itr.value();
        QMap<QString,QString>::iterator subitr=submp.begin();
        QJsonObject subobj;
        while(subitr!=submp.end()){
            subobj.insert(subitr.key(),subitr.value());
            subitr++;
        }
        array.append(subobj);
        itr++;
    }
    TestName.insert("code",this->testCode);
    obj.insert("testName",TestName);
    obj.insert("infoList",array);
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}
