#include "mode_scan.h"
#include "ui_mode_scan.h"
#include <QDateTime>
#include <qmessagebox.h>
mode_scan::mode_scan(QWidget *parent) :
    mode_base (parent),
    ui(new Ui::mode_scan)
{
    this->parent=parent;
    ui->setupUi(this);

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


void mode_scan::next(QString msg){

    input_list[this->at]->setText(msg);
    if(this->at<input_list.size()){
        this->at+=1;
    }
}


void mode_scan::on_lineEdit_step1_returnPressed()
{
    if(this->at==input_list.size()){
        return;
    }
    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>20){
        sl.clear();
    }else{
        QByteArray ary;
        for(int i=0;i<sl.size();i++){
            ary.push_back(sl[i].toLatin1());
        }
        QString s(ary);
        ui->lineEdit_step1->clear();

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

void mode_scan::on_lineEdit_step1_textChanged(const QString &arg1)
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
void mode_scan::on_pushButton_3_clicked()
{
    close();
}
// 撤销上一条
void mode_scan::on_pushButton_2_clicked()
{
    if(at>0){
        at-=1;
        input_list[at]->setText("");
    }
}

mode_scan::~mode_scan()
{
    delete ui;
}

void mode_scan::reset(){
    for (int i=0;i<input_list.size();i++) {
        input_list[i]->clear();
    }
    ui->lineEdit_step1->clear();
}
void mode_scan::init(){

}

QVector<QString>& mode_scan::getSensorMes(){
    return Mes;
}
