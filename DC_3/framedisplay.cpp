﻿#include "framedispaly.h"
#include "ui_framedisplay.h"
#include "dbservice.h"
#include <memory>
#include <windows.h>
#include <mutex>
frameDisplay::frameDisplay(QWidget *parent) : QWidget(parent),
    ui(new Ui::framedisplay_ui)
{
    ui->setupUi(this);

    //以前使用的是tabwidget
    //这里使用的是tabview，而这里使用model来适配tabview

    ui->tableWidget->setColumnCount(8);
    /* 设置列宽在可视界面自适应宽度 */
//    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    /* 行颜色交替显示 */
    ui->tableWidget->setAlternatingRowColors(true);
    /* 不允许在图形界面修改内容 */
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* 设置列数 */

    QStringList stringlists;
    stringlists<<"行号"<<"设备号"<<"帧类型"<<"帧方向"<<"帧ID"<<"帧长度"<<"帧内容"<<"时间戳";
    ui->tableWidget->setHorizontalHeaderLabels(stringlists);

    ui->tableWidget->setColumnWidth(6,300);
    ui->tableWidget->setColumnWidth(7,300);
    rowcount=0;
    allowshow=true;
    receive=true;

    saveInterval_miseconds=settings.saveChart_Interval*60*1000;
    timeClick=GetTickCount();
}

void frameDisplay::getMessage(int mx,CAN_OBJ obj,QString datetime,int companycode){
    //保存信息
    if(!receive){
        return;
    }
    //判断是否超过了预设时间
    if(GetTickCount()-timeClick>saveInterval_miseconds){
        this->on_pushButton_clicked();
    }
    QString messageExtern;
    if(obj.ExternFlag == 1)
    {
        messageExtern = "扩展帧";
    }
    else if(obj.ExternFlag == 0)
    {
        messageExtern = "标准帧";
    }
    FrameType.push_back(messageExtern);
    CompanyName.push_back(settings.CompanyName[companycode]);
    FrameID.push_back(obj.ID);
    FrameLen.push_back(obj.DataLen);
    QString data_info = QString("%1").arg(obj.Data[0], 2, 16, QLatin1Char('0')).toUpper();
    for(int data_long=1;data_long<8;data_long++)
    {
        data_info = data_info + " "+QString("%1").arg(obj.Data[data_long], 2, 16, QLatin1Char('0')).toUpper();
    }
    FrameContent.push_back(data_info);
    Data.push_back(datetime);
    //显示
   if(allowshow==false){
       return;
   }
   ui->tableWidget->insertRow(rowcount);

   std::shared_ptr<QTableWidgetItem> lineid(new QTableWidgetItem(QString::number(mx,10)));
   lineid.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,0,lineid.get());

   std::shared_ptr<QTableWidgetItem> devicetype(new QTableWidgetItem(QString::number(settings.devicetype)));
   devicetype.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,1,devicetype.get());

   std::shared_ptr<QTableWidgetItem> messageExternItem(new QTableWidgetItem(messageExtern));
   messageExternItem.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,2,messageExternItem.get());

   std::shared_ptr<QTableWidgetItem> frametype(new QTableWidgetItem("远程帧|本地帧"));
   frametype.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,3,frametype.get());

   std::shared_ptr<QTableWidgetItem> frameID(new QTableWidgetItem(QString::number(obj.ID,16)));
   frameID.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,4,frameID.get());

   std::shared_ptr<QTableWidgetItem> framelen(new QTableWidgetItem(QString::number(obj.DataLen,10)));
   framelen.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,5,framelen.get());
   //这里写 Data
   std::shared_ptr<QTableWidgetItem> frame_info(new QTableWidgetItem(data_info));
   frame_info.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,6,frame_info.get());

   std::shared_ptr<QTableWidgetItem> frametime(new QTableWidgetItem(datetime));
   frametime.get()->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,7,frametime.get());

   if(rowcount>settings.maxrowcount){
       ui->tableWidget->removeRow(0);
   }else{
       rowcount+=1;
   }
}

void frameDisplay::on_pushButton_2_clicked(){
    if(allowshow==true){
        ui->pushButton_2->setText("继续显示");
    }else{
        ui->pushButton_2->setText("停止显示");
    }
    allowshow=!allowshow;
}

Q_DECLARE_METATYPE(QVector<uint>);
void frameDisplay::on_pushButton_clicked(){
    //保存期间关闭接受
    receive=false;
    //加一层锁
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    QVector<QString> newFrameType;
    QVector<QString> newCompanyName;
    QVector<uint> newFrameID;
    QVector<uint> newFrameLen;
    QVector<QString> newFrameContent;
    QVector<QString> newData;
    newFrameType.swap(FrameType);
    newCompanyName.swap(CompanyName);
    newFrameID.swap(FrameID);
    newFrameLen.swap(FrameLen);
    newFrameContent.swap(FrameContent);
    newData.swap(Data);
//    qDebug()<<newData.size();
//    for(int i=0;i<newData.size();i++){
//        qDebug()<<i<<"  "<<newData[i];
//    }
    QVariant type=QVariant::fromValue(newFrameType);
    QVariant name=QVariant::fromValue(newCompanyName);
    QVariant id=QVariant::fromValue(newFrameID);
    QVariant len=QVariant::fromValue(newFrameLen);
    QVariant content=QVariant::fromValue(newFrameContent);
    QVariant data=QVariant::fromValue(newData);

    emit db.saveTable(content,data,id,type,name,len,
                              QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    timeClick=GetTickCount();
    receive=true;
}
