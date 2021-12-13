#include "sqlprocess.h"
#include "qdebug.h"
#include "QSqlError"

class SqlProcess sqlprocess;

SqlProcess::SqlProcess()
{
//    qDebug()<<QSqlDatabase::isDriverAvailable("QODBC");
//    db=QSqlDatabase::addDatabase("QODBC3");
//    qDebug()<<db.isValid();
//    db.setHostName("127.0.0.1");
//    db.setUserName("root");
//    db.setPassword("wangyang");
//    if(!db.open()){
//        qDebug()<<"db error:"<<db.lastError().text();
//    }else{
//        qDebug()<<"open db success";
//    }

    db=QSqlDatabase::addDatabase("QODBC3");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("sqltest");
    db.setPort(1433);
    db.setUserName("sa");
    db.setPassword("wangyang");
    if(!db.open()){
        qDebug()<<"db error:"<<db.lastError().text();
    }else{
        qDebug()<<"open db success";
    }
}

bool SqlProcess::insert_value(QString command)
{
    QSqlQuery query;
    return query.exec(command);
}

bool SqlProcess::search_value(QVector<QVariant> &result,QString command)
{
    QSqlQuery query;
    if(query.exec(command)){
        return false;
    }else{
        int i=0;
        while(query.next())	//循环输出
        {
            result.push_back(query.value(i++));
        }
        return true;
    }

}

QString SqlProcess::get_lasterror(){
    return db.lastError().text();
}

