#include "settings.h"
#include <qstring.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <qapplication.h>
#include <qdebug.h>
#include <stdio.h>
#include <fstream>
#include <qfileinfo.h>
#include <QMessageBox>

class settings settings;

settings::settings()
{
}

void settings::initConfiguration()
{
    QString dirpath="configuration.ini";
    QFileInfo fileinfo(dirpath);
    if(fileinfo.exists()){
       configuration=new QSettings(dirpath, QSettings::IniFormat);
    }else{
       QMessageBox::information(nullptr,"错误","没有找到配置文件");
       QApplication::exit();
    }
}

void settings::set(QString qstrNodeName, QString qstrKeyName, QVariant qvarvalue){
    configuration->setValue(QString("/%1/%2").arg(qstrNodeName).arg(qstrKeyName), qvarvalue);
}

QVariant settings::get(QString qstrNodeName,QString qstrKeyName)
{
    QVariant qvar = configuration->value(QString("/%1/%2").arg(qstrNodeName).arg(qstrKeyName));
    return qvar;
}


