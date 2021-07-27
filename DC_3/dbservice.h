#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <qthread.h>
#include <qvector.h>
#include <iostream>
#include <fstream>
#include <qdebug.h>
#include <QDateTime>
#include <include/libxl/libxl.h>
class saveChart:public QObject
{
    Q_OBJECT
signals:
public slots:
    void insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date);
public:
    saveChart();

private:
    std::ofstream file;
};

class saveTable:public QObject
{
    Q_OBJECT
signals:
public slots:
    void insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date);
public:
    saveTable();
private:
    std::ofstream file;
};

class Dbservice: public QObject
{
    Q_OBJECT
public:
    Dbservice();
    ~Dbservice();
    void init();
    bool ChartThreadIsAlive();
    bool TableThreadIsAlive();
signals:
    void saveChart(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date);
    void saveTable(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date);
public slots:
private:
   QThread chartThread;
   QThread tableThread;
   class saveChart savechart;
   class saveTable savetable;
};
static Dbservice db;
#endif // DBSERVICE_H
