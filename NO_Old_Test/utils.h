#ifndef UTILS_H
#define UTILS_H

#include <qobject.h>
#include <qwidget.h>
#include <qdialog.h>
#include <QString>
#include <QMap>
#include <qvector.h>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class mode_base:public QDialog
{
    Q_OBJECT
public:
    explicit mode_base(QWidget *parent);
    virtual ~mode_base();
    virtual void reset()=0;
    virtual void init()=0;
};

class MqttDataStruct
{
private:
    QJsonArray ary;
    int testCode=-1;
public:
    MqttDataStruct();
    ~MqttDataStruct();
//    void AddTestName(int code);
    void AddObject(QJsonObject &obj);
    QByteArray tojson();
};
#endif // UTILS_H
