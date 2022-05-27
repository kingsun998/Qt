#ifndef SQLPROCESS_H
#define SQLPROCESS_H

#include <QSqlDatabase>
#include <QSqlQuery>

//extern class SqlProcess sqlprocess;

class SqlProcess
{
public:
    SqlProcess();
    bool insert_value(QString command);
    bool search_value(QVector<QVariant> &result,QString command);
private:
    QSqlDatabase db;

public:
    QString get_lasterror();
};

#endif // SQLPROCESS_H
