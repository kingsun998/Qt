#include "baseinfo.h"
#include "ui_baseinfo.h"

baseInfo::baseInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::baseinfo_ui)
{
    ui->setupUi(this);
//    QWidget *base=ui->baseinfo_ui;
}

void baseInfo::on_lineEdit_2_editingFinished()
{
    settings.Bt_temperature=ui->lineEdit_2->text().toDouble();
}

void baseInfo::on_lineEdit_3_editingFinished()
{
    settings.Bt_temperature=ui->lineEdit_3->text().toDouble();
}

void baseInfo::on_lineEdit_4_editingFinished()
{
    settings.saveInterval_minus=ui->lineEdit_4->text().toDouble();
}
