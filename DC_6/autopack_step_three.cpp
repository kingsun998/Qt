#include "autopack_step_three.h"
#include "ui_autopack_step_three.h"
#include <QDateTime>
autopack_step_three::autopack_step_three(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autopack_step_three)
{
    ui->setupUi(this);
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

autopack_step_three::~autopack_step_three()
{
    delete ui;
}

void autopack_step_three::on_lineEdit_textChanged(const QString &arg1)
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

void autopack_step_three::on_lineEdit_returnPressed()
{
//    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
//    if(nowtime-pre_Time>20){
//        sl.clear();
//    }else{
//        char ary[sl.size()];
//        for(int i=0;i<sl.size();i++){
//            ary[i]=sl[i].toLatin1();
//        }
//        QString s=QString::fromLocal8Bit(ary,sl.size());
//        ui->lineEdit->setText("");
//        if(rowComplete){
//            rowcount++;
//            ui->tableWidget->insertRow(rowcount);
//            QTableWidgetItem *id=new QTableWidgetItem(s);
//            id->setTextAlignment(Qt::AlignHCenter);
//            ui->tableWidget->setItem(rowcount,1,id);
//            rowComplete=false;
//        }
//        else{
//            return;
//        }

//    }
//    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}
