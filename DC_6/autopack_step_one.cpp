#include "autopack_step_one.h"
#include "ui_autopack_step_one.h"
#include <QDateTime>
#include <mqttcommunication.h>
#include <wzserialport.h>
autopack_step_one::autopack_step_one(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autopack_step_one)
{
    ui->setupUi(this);

    /* 行颜色交替显示 */
    ui->tableWidget->setAlternatingRowColors(true);
    /* 不允许在图形界面修改内容 */
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /* 设置列数 */

    QStringList stringlists;
    stringlists<<"行号"<<"主板ID"<<"温度高位"<<"温度低位";
    ui->tableWidget->setColumnCount(stringlists.size());
    ui->tableWidget->setHorizontalHeaderLabels(stringlists);

    rowcount=-1;
    rowComplete=true;
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();

    wzserialport_multiple.setPortStruct(&multipleprocessstruct);
    connect(&multipleprocessstruct,&MultipleProcessStruct::finish,this,&autopack_step_one::load_mes);


}

autopack_step_one::~autopack_step_one()
{
    delete ui;
}
void autopack_step_one::load_mes(){
    if(rowComplete){
        return;
    }else{
        // rowcomplete 为false，即不完整
        rowComplete=true;
        QTableWidgetItem *high=new QTableWidgetItem(
                    QString::number(multipleprocessstruct.highTemperature));
        high->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(rowcount,2,high);

        QTableWidgetItem *low=new QTableWidgetItem(
                    QString::number(multipleprocessstruct.lowTemperature));
        low->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(rowcount,2,low);
    }
}
void autopack_step_one::on_lineEdit_textChanged(const QString &arg1)
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

void autopack_step_one::on_lineEdit_returnPressed()
{
    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>20){
        sl.clear();
    }else{
        char ary[sl.size()];
        for(int i=0;i<sl.size();i++){
            ary[i]=sl[i].toLatin1();
        }
        QString s=QString::fromLocal8Bit(ary,sl.size());
        ui->lineEdit->setText("");
        if(rowComplete){
            rowcount++;
            ui->tableWidget->insertRow(rowcount);
            QTableWidgetItem *id=new QTableWidgetItem(s);
            id->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget->setItem(rowcount,1,id);
            rowComplete=false;
        }
        else{
            return;
        }

    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

// 获取数据
void autopack_step_one::on_pushButton_3_clicked()
{
    emit wzserialport_multiple.send_multiple_receive();
}
// 撤销上一条
void autopack_step_one::on_pushButton_clicked()
{
    if(rowcount>=0){
        ui->tableWidget->removeRow(rowcount);
        rowcount--;
        rowComplete=true;
    }
}
// 发送数据
void autopack_step_one::on_pushButton_2_clicked()
{
     MqttDataStruct date;
     date.AddTestName(0);
     date.AddObject("item1","id",ui->tableWidget->item(rowcount,1)->text());
     date.AddObject("item1","id",ui->tableWidget->item(rowcount,2)->text());
     date.AddObject("item1","id",ui->tableWidget->item(rowcount,3)->text());
     mqtt.publish_Mes(date);
}

void autopack_step_one::on_pushButton_4_clicked()
{
    ui->comboBox->clear();
    QStringList &&comname= wzserialport_multiple.get_available_list();
    ui->comboBox->addItems(comname);

    if(comname.size()!=0){
        wzserialport_multiple.open(comname[0].toLatin1().data(),115200,0,8,1,1);
    }


}
