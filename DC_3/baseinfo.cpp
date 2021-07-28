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

void baseInfo::on_lineEdit_5_editingFinished()
{
    settings.maxrowcount=ui->lineEdit_5->text().toInt();
}

void baseInfo::on_comboBox_8_activated(int index)
{
    QString s=ui->comboBox_8->itemText(index);
    settings.boterate=s.mid(0,s.size()-4).toInt();
    settings.setbote();
}
