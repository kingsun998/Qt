#include "usbcanunion.h"
#include "settings.h"
#include "QTime"

class UsbCanUnion usbcanunion;
UsbCanUnion::UsbCanUnion()
{

}

DWORD UsbCanUnion::openUsbCan(){
    return OpenDevice(settings.devicetype,settings.deviceid,0);
}
DWORD UsbCanUnion::closeUsbCan(){
    return CloseDevice(settings.devicetype,settings.deviceid);
}

DWORD UsbCanUnion::initUsbCan(){
    return InitCAN(settings.devicetype,settings.deviceid,settings.canid,&settings.pInitConfig);
}

DWORD UsbCanUnion::startUsbCan(){
    return StartCAN(settings.devicetype, settings.deviceid, settings.canid);
}

ULONG UsbCanUnion::ReceiveFromCan(){
    return Receive(settings.devicetype,settings.deviceid,settings.canid,objs,50,100);
}

ULONG UsbCanUnion::SendToCan(){
//    return Receive(settings.devicetype,settings.deviceid,settings.canid,objs,50,100);
    return 0;
}

ULONG UsbCanUnion::StartTestMode(){
    ULONG len=10;
    QVector<int> a_objs_id={
                          0x18FD2083,0x14FD3E83,0x18FF7AD3,0x18FDB483,0x10FD4783,
                          0x18FD2084,0x14FD3E84,0x18FF7AD4,0x18FDB484,0x10FD4784
                         };
    QVector<int> b_objs_id={
                          0x18FF76D3,0x18FF77D3,0x18FF74D3,0x18FF75D3,0x10FD47D3,
                          0x18FF76D4,0x18FF77D4,0x18FF74D4,0x18FF75D4,0x10FD47D4
                         };
    QVector<int> objs_id=a_objs_id;
    int size=b_objs_id.size();
    for(ULONG i=0;i<len;i++){
        for(int j=0;j<8;j++){
            // 随机数种子
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
            objs[i].Data[j]= qrand()%5+35;   //随机生成0到9的随机数
        }
        objs[i].DataLen=8;
        objs[i].ID=objs_id[i%size];
    }
    return len;
}


