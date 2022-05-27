#include "subcomdialog.h"
#include "ui_subcomdialog.h"
#include <QDebug>

SubComDialog::SubComDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubComDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);

}

SubComDialog::~SubComDialog()
{
    delete ui;
}

Q_DECLARE_METATYPE(Message)
void SubComDialog::on_pushButton_2_clicked()
{
    Message mes;
    if(!ui->lineEdit->text().isEmpty()){
        mes.heatPlc=ui->lineEdit->text().toInt();
    }
    if(!ui->lineEdit_2->text().isEmpty()){
        mes.coldPlc=ui->lineEdit_2->text().toInt();
    }
    if(!ui->lineEdit_3->text().isEmpty()){
        mes.totalTestPlc=ui->lineEdit_3->text().toInt();
    }
    if(ui->radioButton->isChecked()){
        mes.changeChannelPlc=1;
    }
    if(!ui->lineEdit_5->text().isEmpty()){
        mes.hourPlc=ui->lineEdit_5->text().toInt();
    }
    if(!ui->lineEdit_6->text().isEmpty()){
        mes.minutePlc=ui->lineEdit_6->text().toInt();
    }
    if(!ui->lineEdit_7->text().isEmpty()){
        mes.secondPlc=ui->lineEdit_7->text().toInt();
    }
    if(ui->radioButton_2->isChecked()){
        mes.upHeatPipeMoveDown=1;
    }
    if(ui->radioButton_3->isChecked()){
        mes.leftHeatPipeMoveForward=1;
    }
    QVariant var=QVariant::fromValue(mes);
    emit SendComMes(var);
    this->close();
}

void SubComDialog::on_pushButton_clicked()
{
    this->close();
}
