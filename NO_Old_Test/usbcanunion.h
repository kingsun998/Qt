#ifndef USBCANUNION_H
#define USBCANUNION_H

extern class UsbCanUnion usbcanunion;

#include <QObject>
#include <include/zlgcan.h>
#include <vector>

class UsbCanUnion: public QObject
{
    Q_OBJECT
public:
    UsbCanUnion();

signals:
    void setFrameStartTime();

public slots:
    int openInitUsbCan();
    int closeUsbCan();
    int startUsbCan(unsigned int channelid);
    uint ReceiveFromCan(uint channelid);
    int SendToCan(uint channelid);
    int StartTestMode();
public:
//    CAN_OBJ objs[50];
private:
    DEVICE_HANDLE dhandle;
    std::vector<CHANNEL_HANDLE> chHandle;
    ZCAN_CHANNEL_INIT_CONFIG cfg;
    ZCAN_Transmit_Data send_frame;
    ZCAN_Receive_Data receive_data;
};

#endif // USBCANUNION_H
