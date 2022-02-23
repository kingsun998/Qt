#include "deviceunion.h"
#include "ui_deviceunion.h"
#include "qdebug.h"
#include "sqlprocess.h"
DeviceUnion::DeviceUnion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceUnion)
{
    ui->setupUi(this);

    at=0;
    input_list.push_back(ui->label_4);
    input_list.push_back(ui->label_5);
    input_list.push_back(ui->label_6);
    for(int i=0;i<input_list.size();i++){
        input_list[i]->setEnabled(true);
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
    pre_mes="";
    sl.clear();
}


DeviceUnion::~DeviceUnion()
{
    delete ui;
}

void DeviceUnion::on_pushButton_2_clicked()
{
    for (int i=0;i<input_list.size();i++) {
        input_list[i]->setText("");
    }
    at=0;
}

void DeviceUnion::on_pushButton_3_clicked()
{
    QString mb=input_list[0]->text();
    QString sb_1=input_list[1]->text();
    QString sb_2=input_list[2]->text();
    if(!sqlprocess.insert_value(QString("insert into A (device_id) values(%1)").arg(mb))){
        qDebug()<<"插入 Main block 失败";
        qDebug()<<sqlprocess.get_lasterror();
    }
    if(!sqlprocess.insert_value(QString("insert into B (device_id,Pid) values(%1,%2)").arg(sb_1).arg(mb))){
        qDebug()<<"插入 sub block 1 失败";
        qDebug()<<sqlprocess.get_lasterror();
    }
    if(!sqlprocess.insert_value(QString("insert into B (device_id,Pid) values(%1,%2)").arg(sb_1).arg(mb))){
        qDebug()<<"插入 sub block 2 失败";
        qDebug()<<sqlprocess.get_lasterror();
    }
    for (int i=0;i<input_list.size();i++) {
        input_list[i]->setText("");
    }
    at=0;
}

void DeviceUnion::next(QString msg){
    input_list[at]->setText(msg);
    if(at<input_list.size()){
        at+=1;
    }
}


void DeviceUnion::on_lineEdit_4_returnPressed()
{
    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>10){
        sl.clear();
    }else{
        char ary[sl.size()];
        for(int i=0;i<sl.size();i++){
            ary[i]=sl[i].toLatin1();
        }
        QString s=QString::fromLocal8Bit(ary,sl.size());
        qDebug()<<"长度:"<<s.length();
        ui->lineEdit_4->setText("");
//        ui->label_4->setText(s);
        next(s);
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug()<<"std::endl;";
}

void DeviceUnion::on_lineEdit_4_textChanged(const QString &arg1)
{
    std::lock_guard<std::mutex> lk(lock);

    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug()<<nowtime<<"    "<<pre_Time;
    if(nowtime-pre_Time>50){
       qDebug()<<"超时";
       sl.clear();
       sl.push_back(*(arg1.end()-1));
    }else{
       sl.push_back(*(arg1.end()-1));
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void DeviceUnion::on_groupBox_clicked()
{
     ui->lineEdit_4->setFocus();
}

