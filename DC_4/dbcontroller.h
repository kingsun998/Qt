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
#include "libxl/libxl.h"
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
    explicit dbSingleChart(QMap<QString,QMap<QString,QVector<QString>>> &tp,QVector<QString> &time,int company_type);
    void  createSingleChartXlsx(libxl::Sheet *sheet);
    void save();
public:
    QMap<QString,QMap<QString,QVector<QString>>> map;
    QString date;
    int company_type;

    QVector<QString> time;
    QVector<QString> title_col;
    QVector<QString> use_dates;
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
