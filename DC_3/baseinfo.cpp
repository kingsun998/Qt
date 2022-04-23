#include "baseinfo.h"
#include "ui_baseinfo.h"
#include "qdebug.h"
#include <mqttcommunication.h>
#include <qmessagebox.h>
baseInfo::baseInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::baseinfo_ui)
{
    ui->setupUi(this);

    settings.setbote();
    pushbutton_list.append(ui->pushButton);
    pushbutton_list.append(ui->pushButton_2);
    pushbutton_list.append(ui->pushButton_3);
    for (int i=0;i<pushbutton_list.size();i++) {
        pushbutton_list[i]->setFont(*settings.pushbutton_font);
        pushbutton_list[i]->setMinimumSize(130,40);
    }
}

void baseInfo::on_lineEdit_2_editingFinished()
{
    QString num=ui->lineEdit_2->text();
    if(num!=""||num!=nullptr){
        settings.Bt_temperature=num.toDouble();
    }
}

void baseInfo::on_lineEdit_3_editingFinished()
{
    QString num=ui->lineEdit_3->text();
    if(num!=""||num!=nullptr){
        settings.Tp_temperature=num.toDouble()*0.63;
    }
}

// 修改图表保存时间间隔
void baseInfo::on_lineEdit_4_editingFinished()
{
    QString num=ui->lineEdit_4->text();
    if(num!=""||num!=nullptr){
        qDebug()<<"更新图表保存时间"<<num.toDouble();
        settings.saveInterval_minus=num.toDouble();
    }else{
        qDebug()<<"请输入保存时间";
    }
}

void baseInfo::on_lineEdit_6_editingFinished()
{
    QString num=ui->lineEdit_6->text();

    if(num!=""||num!=nullptr){
        qDebug()<<"更新保存表格时间"<<num.toDouble();
        settings.saveTable_Interval=num.toDouble();
    }else{
        qDebug()<<"请输出保存时间";
    }
}

void baseInfo::on_lineEdit_5_editingFinished()
{
    QString num=ui->lineEdit_5->text();
    if(num!=""||num!=nullptr){
        settings.maxrowcount=num.toInt();
    }
//    qDebug()<<&settings;

}

void baseInfo::on_comboBox_8_activated(int index)
{
    QString s=ui->comboBox_8->itemText(index);
    settings.boterate=s.mid(0,s.size()-4).toInt();
    settings.setbote();
}



void baseInfo::on_radioButton_clicked(bool checked)
{
    qDebug()<<checked;
    settings.TestMode=checked;
}

//void baseInfo::on_pushButton_clicked()
//{
//    mqtt.init(,ui->lineEdit_8->text(),ui->lineEdit_9->text());
//}

void baseInfo::on_pushButton_2_clicked()
{
    mqtt.connectToHost();
}



void baseInfo::on_pushButton_3_clicked()
{
    mqtt.scribe();
}

void baseInfo::on_pushButton_clicked()
{
    QString ip=ui->lineEdit_7->text()==""?settings.mqtt_ip_address:ui->lineEdit_7->text();
    QString port=ui->lineEdit_8->text()==""?QString::number(settings.mqtt_port):ui->lineEdit_8->text();
    QString topic=ui->lineEdit_9->text()==""?settings.topic:ui->lineEdit_9->text();
    mqtt.init(ip,port,topic);
}

void baseInfo::on_lineEdit_10_editingFinished()
{
    settings.baurd=ui->lineEdit_10->text().toInt();
}

void baseInfo::on_lineEdit_11_editingFinished()
{
    settings.stopbit=ui->lineEdit_11->text().toInt();
}

void baseInfo::on_lineEdit_12_editingFinished()
{
    settings.check=ui->lineEdit_12->text().toInt();
}

void baseInfo::on_lineEdit_13_editingFinished()
{
    settings.datalen=ui->lineEdit_13->text().toInt();
}
