#ifndef USBCANUNION_H
#define USBCANUNION_H

extern class UsbCanUnion usbcanunion;

#include <QObject>
#include <ECanVci.h>


class UsbCanUnion: public QObject
{
    Q_OBJECT
public:
    UsbCanUnion();

signals:
    void setFrameStartTime();

public slots:
    DWORD openUsbCan();
    DWORD closeUsbCan();
    DWORD initUsbCan();
    DWORD startUsbCan();
    ULONG ReceiveFromCan();
    ULONG SendToCan(CAN_OBJ &send);
    ULONG StartTestMode();
    ULONG StartUnionTestMode();
public:
    CAN_OBJ objs[50];
};

#endif // USBCANUNION_H
