#include "framedisplay.h"
#include "ui_framedisplay.h"
#include <memory>
#include <windows.h>
#include <mutex>
#include <qmessagebox.h>
frameDisplay::frameDisplay(QWidget *parent) : QWidget(parent),
    ui(new Ui::framedisplay_ui)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(8);

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
    dispaly_allowshow=true;
    dispaly_ifreceive=true;

    saveInterval_miseconds=settings.saveTable_Interval*60*1000;


    send_frame_type=settings.send_frame_type[0];
    send_frame_format=settings.send_frame_format[0];
    send_frame_id=0;
//    send_frame_interval=10000;
    send_frame_nums=1;

    sendtimer=new QTimer();
    sendtimer->setInterval(100);
}


void frameDisplay::getMessage(ulong index,CAN_OBJ obj,QString datetime,QString companyname){
    //保存信息
    if(!dispaly_ifreceive){
        return;
    }
    //判断是否超过了预设时间
    if(GetTickCount()-startTime>saveInterval_miseconds){
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
    ifFrameReceive.push_back(true);

    CompanyName.push_back(companyname);
    FrameID.push_back(obj.ID);
    FrameLen.push_back(obj.DataLen);
    QString data_info = QString("%1").arg(obj.Data[0], 2, 16, QLatin1Char('0')).toUpper();
    for(int data_long=1;data_long<8;data_long++)
    {
        data_info = data_info + " "+QString("%1").arg(obj.Data[data_long], 2, 16, QLatin1Char('0')).toUpper();
    }
    FrameContent.push_back(data_info);
    Data.push_back(datetime);
    QString ReceiveOrSend;
    ReceiveOrSend="接受帧";
    //显示
   if(dispaly_allowshow==false){
       return;
   }
   ui->tableWidget->insertRow(rowcount);

   QTableWidgetItem *lineid=new QTableWidgetItem(QString::number(index,10));
   lineid->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,0,lineid);


   QTableWidgetItem *cpname=new QTableWidgetItem(companyname);
   cpname->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,1,cpname);


   QTableWidgetItem *messageExternItem=new QTableWidgetItem(messageExtern);
   messageExternItem->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,2,messageExternItem);


   QTableWidgetItem *frameReceiveOrsend=new QTableWidgetItem(ReceiveOrSend);
   frameReceiveOrsend->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,3,frameReceiveOrsend);

   QTableWidgetItem *frameID=new QTableWidgetItem(QString::number(obj.ID,16));
   frameID->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,4,frameID);

   QTableWidgetItem *framelen=new QTableWidgetItem(QString::number(obj.DataLen,10));
   framelen->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,5,framelen);

   //这里写 Data
   QTableWidgetItem *df=new QTableWidgetItem(data_info);
   df->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,6,df);

   QTableWidgetItem *dt=new QTableWidgetItem(datetime);
   dt->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,7,dt);

   if(rowcount>settings.maxrowcount){
       ui->tableWidget->removeRow(0);
   }else{
       rowcount+=1;
   }

}

void frameDisplay::on_pushButton_2_clicked(){
    if(dispaly_allowshow==true){
        ui->pushButton_2->setText("继续显示");
    }else{
        ui->pushButton_2->setText("停止显示");
    }
    dispaly_allowshow=!dispaly_allowshow;
}

void frameDisplay::setStartTime(){
    showMessage("设置了开始时间",true);
    this->startTime=GetTickCount();
}

Q_DECLARE_METATYPE(QVector<uint>);
void frameDisplay::on_pushButton_clicked(){
    //保存期间关闭接受
//    qDebug()<<&settings;
    dispaly_ifreceive=false;
    //加一层锁
    std::lock_guard<std::mutex> lock(mtx);

    dbSingleTable *obj=new dbSingleTable(FrameContent,Data,FrameID,FrameType,CompanyName,FrameLen,
                                         ifFrameReceive,QDateTime::currentDateTime().toString("yyyy_MM_dd"));
    dbcontroller.addObject(obj);
    startTime=GetTickCount();
    dispaly_ifreceive=true;
}
