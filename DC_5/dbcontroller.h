#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QQueue>
#include <qvector.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qdebug.h>
#include <QCoreApplication>
#include <settings.h>
#include <QThread>
#include <wzserialport.h>
extern class dbController dbcontroller;

static qint64  string2datetimeSeconds(QString s){
    return QDateTime::fromString(s,"yyyy_MM_dd_hh").currentSecsSinceEpoch();
}

static QString getCurrentDateTime(){
    return QDateTime::currentDateTime().toString("yyyy_MM_dd_hh");
}

static QString getCurrentDate(){
    return QDateTime::currentDateTime().toString("yyyy_MM_dd");
}


class dbBaseObject : public QObject
{
    Q_OBJECT
signals:
    void finish();
public:
    explicit dbBaseObject(QObject *parent = nullptr);
    virtual void save();
};


class dbSingleChart : public dbBaseObject
{
    Q_OBJECT
public:
    explicit dbSingleChart(QMap<QString,QMap<QString,QVector<QString>>> &tp,QVector<QString> &time,QString compandy_type);

    void save();
public:
    QMap<QString,QMap<QString,QVector<QString>>> map;
    QString date;
    QString company_type;

    QVector<QString> time;
    QVector<QString> title_col;
    QVector<QString> use_dates;
};

class dbDoubleChart : public dbBaseObject
{
    Q_OBJECT
public:
    explicit dbDoubleChart(QMap<QString,QMap<QString,QVector<QString>>> &tp1,
                           QMap<QString,QMap<QString,QVector<QString>>> &tp2,
                           QVector<QVector<QString>> &time,
                           QString date,QString compandy_type);

    void save();

public:
    QVector<QMap<QString,QMap<QString,QVector<QString>>>>  map;
    QVector<QVector<QString>> time;
    QString compandy_type;
    QVector<QString> title_col;
    QVector<QString> use_dates;
};

class dbSinglePortTest : public dbBaseObject
{
    Q_OBJECT
public:
    explicit dbSinglePortTest(QVector<SinglePortStruct>& vec,QVector<QString>& sensorId,QString type);
    void save();

public:
    QVector<SinglePortStruct>  vector;
    QVector<QString> sensorId;
    QString  type;
};


class dbSingleTable : public dbBaseObject
{
    Q_OBJECT
public:
    explicit dbSingleTable(QVector<QString> &content,QVector<QString> &timestamp,QVector<uint> &id,
                           QVector<QString> &type,QVector<QString> &name,QVector<uint> &len,
                           QVector<bool> &ifsend,QString date);
    void save();
public:
    QVector<QString> content;
    QVector<QString> timestamp;
    QVector<uint> id;
    QVector<QString> type;
    QVector<QString> name;
    QVector<uint> len;
    QVector<bool> ifsend;
    QString date;
};

class dbMergeFile : public dbBaseObject
{
    Q_OBJECT
public:
    explicit dbMergeFile(QString tempDir,QString targetDir);
    void save();
public:
    QString tempDir;
    QString targetDir;
    QString savename;
};


class dbController : public QObject
{
    Q_OBJECT
public:
    explicit dbController(QObject *parent = nullptr);
    void addObject(dbBaseObject* obj);
    ~dbController();
public slots:
    void writeFinish();
    void CheckWrite();
public :
    QQueue<dbBaseObject* > TaskQueue;
    QThread  thread;
    std::mutex mtx;
    bool ifruning;
};

#endif // DBCONTROLLER_H
