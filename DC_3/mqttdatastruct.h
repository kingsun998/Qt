#ifndef MQTTDATESTRUCT_H
#define MQTTDATESTRUCT_H

#include <QString>
#include <QMap>
#include <qvector.h>
class MqttDataStruct
{
private:
    QMap<QString,QMap<QString,QString>> map;
public:
    MqttDataStruct();
    void AddObject(QString name,QString properties,const QString val);
    QByteArray tojson();
};

#endif // MQTTDATESTRUCT_H
