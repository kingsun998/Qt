#include "scangun.h"
#include "qdebug.h"
#include "ui_scangun.h"
#include "sqlprocess.h"
#include <testdisplay.h>
#include <QMessageBox>
#include "QDateTime"
ScanGun::ScanGun(QWidget *parent,QVector<QString> *mes) :
    QDialog(parent),
    Mes(mes),
    ui(new Ui::ScanGun)
{
    this->parent=parent;
    ui->setupUi(this);

    ui->tabWidget->setTabText(0,"扫描");

    at=0;
    input_list.push_back(ui->label_step1_0);
    input_list.push_back(ui->label_step1_1);
    input_list.push_back(ui->label_step1_2);
    input_list.push_back(ui->label_step1_3);
    input_list.push_back(ui->label_step1_4);
    input_list.push_back(ui->label_step1_5);
    input_list.push_back(ui->label_step1_6);
    input_list.push_back(ui->label_step1_7);

    lineedit_list.push_back(ui->lineEdit_step1);


    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
    pre_mes="";
    at=0;
    sl.clear();
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    sensorId.insert(" ");
    ui->lineEdit_step1->setFocus();
}


ScanGun::~ScanGun()
{
    delete ui;
}


void ScanGun::next(QString msg){

    input_list[this->at]->setText(msg);
    if(this->at<input_list.size()){
        this->at+=1;
    }
}

void ScanGun::getSensorMes(){
    Mes->clear();
    for(int i=0;i<at;i++){
        Mes->append(input_list[i]->text());
    }
}

void ScanGun::on_lineEdit_step1_returnPressed()
{
    if(this->at==input_list.size()){
        return;
    }
    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>20){
        sl.clear();
    }else{
        char ary[sl.size()];
        for(int i=0;i<sl.size();i++){
            ary[i]=sl[i].toLatin1();
        }
        QString s=QString::fromLocal8Bit(ary,sl.size());
        ui->lineEdit_step1->setText("");

        QSet<QString>::const_iterator itr=sensorId.constEnd();
        if(sensorId.find(s)==itr){
            sensorId.insert(s);
            next(s);
        }
        else{
            QMessageBox::critical(nullptr,"错误","传感器ID重复");
        }
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void ScanGun::on_lineEdit_step1_textChanged(const QString &arg1)
{
    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>30){
       sl.clear();
       sl.push_back(*(arg1.end()-1));
    }else{
       sl.push_back(*(arg1.end()-1));
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

//扫描完成
void ScanGun::on_pushButton_3_clicked()
{
    getSensorMes();
    close();
}
// 撤销上一条
void ScanGun::on_pushButton_2_clicked()
{
    qDebug()<<"remove last";
    if(at>0){
        at-=1;
        input_list[at]->setText("");
    }
}

void ScanGun::reject(){
    emit closeTheDialog();
    QDialog::reject();
}
