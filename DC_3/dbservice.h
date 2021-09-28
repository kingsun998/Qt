#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <qthread.h>
#include <qvector.h>
#include <fstream>
#include <qdebug.h>
#include <QDateTime>
#include <settings.h>
#include <qdir.h>
#include <iostream>
#include <libxl/libxl.h>
#include <conio.h>

extern class Dbservice db;
class saveChart:public QObject
{
    Q_OBJECT
signals:
public slots:
    void insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date);
public:
    saveChart();

private:
    int current_page=-1;
    void createXlsx(QString filename);
    void createSheet(libxl::Book *book);
    std::ofstream file;
};

class saveTable:public QObject
{
    Q_OBJECT
signals:
public slots:
    void insert(QVariant content,QVariant timestamp,QVariant id,
                QVariant type,QVariant name,QVariant len,QString date);
public:
    saveTable();
private:
    int current_page=-1;
    void createXlsx(QString filename);
    void createSheet(libxl::Book *book);
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
    void saveTable(QVariant content,QVariant timestamp,QVariant id,
                   QVariant type,QVariant name,QVariant len,QString date);
public slots:
private:
   QThread chartThread;
   QThread tableThread;
   class saveChart savechart;
   class saveTable savetable;
   int current_date;
};

#endif // DBSERVICE_H
