#include "baseinfo.h"
#include "ui_baseinfo.h"
#include "qdebug.h"

baseInfo::baseInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::baseinfo_ui)
{
    ui->setupUi(this);
//    QWidget *base=ui->baseinfo_ui;
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

void baseInfo::on_lineEdit_6_editingFinished()
{
    QString num=ui->lineEdit_6->text();
    if(num!=""||num!=nullptr){
        settings.saveTable_Interval=num.toDouble();
    }
}

void baseInfo::on_radioButton_clicked(bool checked)
{
    qDebug()<<checked;
    settings.TestMode=checked;
}
