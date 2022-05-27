#include "messagedisplay.h"
#include "ui_messagedisplay.h"
#include <usbcanunion.h>

MessageDisplay::MessageDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageDisplay)
{
    ui->setupUi(this);
    index=0;

    QStringList stringlists;
    stringlists<<"序号"<<"时间标识"<<"原通道"<<"帧ID"<<"CAN类型"<<"方向"<<"长度"<<"数据";
    ui->tableWidget->setHorizontalHeaderLabels(stringlists);


}

MessageDisplay::~MessageDisplay()
{
    delete ui;
}
