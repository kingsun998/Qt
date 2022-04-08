#ifndef TRANSPORTOBJECTS_H
#define TRANSPORTOBJECTS_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QString>
#include <QLabel>
#include <QVariant>

class DataStruct
{
public:
    DataStruct();
    ~DataStruct();
    void reset();
public:
    QMap<QString,QMap<QString,QVector<QString>>>  obj;
    void setParameter(QString conpany_type);
    QVariant getvariant();
    QString company_type;
private:
    QVector<QString> use_names;
    QVector<QString> ttnames;
    QVector<QString> cpnames;


};


#endif // TRANSPORTOBJECTS_H
