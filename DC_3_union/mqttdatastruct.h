#ifndef MQTTDATESTRUCT_H
#define MQTTDATESTRUCT_H

#include <QString>
#include <QMap>
#include <qvector.h>
#include <QList>

class MqttDataStruct
{
private:
    QMap<QString,QMap<QString,QString>> map;
    int testCode=-1;
public:
    MqttDataStruct();
    void AddTestName(int code);
    void AddObject(QString name,QString properties,const QString val);
    QByteArray tojson();
};

#endif // MQTTDATESTRUCT_H
