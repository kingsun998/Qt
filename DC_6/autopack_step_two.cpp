#include "autopack_step_two.h"
#include "ui_autopack_step_two.h"
#include <QDateTime>
autopack_step_two::autopack_step_two(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autopack_step_two)
{
    ui->setupUi(this);
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

autopack_step_two::~autopack_step_two()
{
    delete ui;
}

void autopack_step_two::on_lineEdit_textChanged(const QString &arg1)
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

void autopack_step_two::on_lineEdit_returnPressed()
{
//    if(this->at==input_list.size()){
//        return;
//    }
//    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
//    if(nowtime-pre_Time>20){
//        sl.clear();
//    }else{
//        char ary[sl.size()];
//        for(int i=0;i<sl.size();i++){
//            ary[i]=sl[i].toLatin1();
//        }
//        QString s=QString::fromLocal8Bit(ary,sl.size());
//        ui->lineEdit_step1->setText("");

//        QSet<QString>::const_iterator itr=sensorId.constEnd();
//        if(sensorId.find(s)==itr){
//            sensorId.insert(s);
//            next(s);
//        }
//        else{
//            QMessageBox::critical(nullptr,"错误","传感器ID重复");
//        }
//    }
//    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}
