#ifndef SENDMESSAGES_H
#define SENDMESSAGES_H

#include <QWidget>
#include <settings.h>
#include "include/usb2xxx/usb_device.h"
#include "include/usb2xxx/usb2can.h"

#include <QTimer>
#include <fstream>
#include <io.h>
#include <iostream>
#include <QFileDialog>
namespace Ui {
    class sendmessages_ui;
}
class SendMessages :public QWidget
{
    Q_OBJECT
public:
    explicit SendMessages(QWidget * parent =nullptr);
    ~SendMessages();
    int sendmessage_schedule();

signals:
    void updateProcess(int val);

private slots:
    void on_pushButton_clicked();
    void handleTimeOut();
    void on_pushButton_2_clicked();
    void on_lineEdit_2_editingFinished();

    void on_pushButton_3_clicked();


private:
    Ui::sendmessages_ui *ui;
    DEVICE_INFO devinfo;

    DEVICE_INFO DevInfo;
    //    每个设备的设备号存储地址
    int DevHandle[10];
    //0-CAN1,1-CAN2
    DWORD CANIndex;

    QTimer *timer;
    std::ifstream *file;

    int totallength;
    int currentlength;
    uint baseID;

    bool ifsending;

    int init();
    int initDevice(int* DevHandle);
    int getDeviceInfo(int DevAddress,DEVICE_INFO &info);
    int initCan(int DevAddress);
    int initFilter(int DevAddress);
    int getStatus(int DevAddress);
};

#endif // SENDMESSAGES_H
