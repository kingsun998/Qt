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
QByteArray MqttDataStruct::tojson(){
    QMap<QString,QMap<QString,QString>>::iterator itr=map.begin();
    QJsonObject obj;
    while (itr!=map.end()) {
        auto submp=itr.value();
        QMap<QString,QString>::iterator subitr=submp.begin();
        QJsonObject subobj;
        while(subitr!=submp.end()){
            subobj.insert(subitr.key(),subitr.value());
            subitr++;
        }
        obj.insert(itr.key(),subobj);
        itr++;
    }
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}
