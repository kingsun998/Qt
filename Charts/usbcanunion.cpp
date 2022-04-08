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

ULONG UsbCanUnion::SendToCan(CAN_OBJ &send){
//    DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,P_CAN_OBJ pSend,ULONG Len
    return Transmit(settings.devicetype,settings.deviceid,settings.canid,&send,1);
}

ULONG UsbCanUnion::StartTestMode(){

    byte mes[8][8]={
        {0x57, 0x35, 0xB8, 0x37, 0xFF, 0xFF, 0xE0, 0xFD},
        {0x7B, 0x35, 0x67, 0x35, 0xFF, 0xFF, 0xAD, 0xFD},
        {0x87, 0x35, 0x8F, 0x35, 0x7B, 0x35, 0xAD, 0xB5},
        {0x57, 0x35, 0xB8, 0x37, 0xFF, 0xFF, 0xE0, 0xFD},
        {0xB4, 0x35, 0x19, 0x38, 0xFF, 0xFF, 0xE0, 0xFD},
        {0x95, 0x35, 0x95, 0x35, 0xFF, 0xFF, 0xAD, 0xFD},
        {0x91, 0x35, 0x99, 0x35, 0x95, 0x35, 0xAD, 0xB5},
        {0xB4, 0x35, 0x19, 0x38, 0xFF, 0xFF, 0xE0, 0xFD},
    };
    byte mes2[][4]={{0x05, 0x48, 0x0e, 0x3c}};

    QVector<int> a_objs_id={
                          0x18FD2083,0x14FD3E83,0x18FF7AD3,0x18FDB483,
                          0x18FD2084,0x14FD3E84,0x18FF7AD4,0x18FDB484,
                         };//0x10FD4783,  0x10FD4784
    QVector<int> b_objs_id={
                          0x18FF75D3,0x18FF76D3,0x18FF74D3,0x18FF77D3,
                          0x18FF75D4,0x18FF76D4,0x18FF74D4,0x18FF77D4
                         };//0x10FD47D3,   0x10FD47D4
    QVector<int> c_objs_id={0x18FF75D3};

    QVector<int> objs_id=c_objs_id;
    ULONG len=objs_id.size();
    for(ULONG i=0;i<len;i++){
        memmove(objs[i].Data,mes2[i],4);
        objs[i].DataLen=4;
        objs[i].ID=objs_id[i];
    }
    return len;
}


