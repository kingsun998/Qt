#include "usbcanunion.h"
#include "settings.h"
#include "QTime"
#include <QMessageBox>
#include <settings.h>
class UsbCanUnion usbcanunion;
UsbCanUnion::UsbCanUnion()
{

}
// 开启及初始化盒子
int UsbCanUnion::openInitUsbCan(){
    // 读取配置
    // 打开设备
    uint device_type=settings.get("usbcan","device_type").toUInt();
    uint device_index=settings.get("usbcan","device_index").toUInt();
    dhandle=ZCAN_OpenDevice(device_type,device_index,0);
    if(dhandle==INVALID_DEVICE_HANDLE){
        QMessageBox::information(nullptr,"错误","打开设备失败");
        return -1;
    }
    // 设置盒子参数
    std::string path=std::to_string(device_index)+"/baud_rate";
    const char *baud=settings.get("usbcan","init_baud").toByteArray().data();
    if(ZCAN_SetValue(dhandle,path.c_str(),baud)!=STATUS_OK)
    {
        QMessageBox::information(nullptr,"错误","设置波特率失败");
        return -1;
    }
    // 设置通道参数
    memset(&cfg,0,sizeof(cfg));
    cfg.can_type = TYPE_CAN;
    cfg.can.filter=0;
    cfg.can.mode=0;
    cfg.can.acc_code=0;
    cfg.can.acc_mask = 0xffffffff;
    // 初始化不同的通道
    uint channel_size=settings.get("usbcan","channel_size").toUInt();
    chHandle.resize(channel_size);
    for (uint i=0;i<channel_size;i++) {
        chHandle[i]=ZCAN_InitCAN(dhandle,i,&cfg);
        if(INVALID_CHANNEL_HANDLE==chHandle[i]){
             QMessageBox::information(nullptr,"错误","初始化Can通道"+QString::number(i)+"失败");
             return -1;
        }
    }
    return 1;
}
// 为盒子的下一步处理做准备,这里是单个通道进行初始化
int UsbCanUnion::startUsbCan(unsigned int channelid){
    if (ZCAN_StartCAN(chHandle[channelid]) != STATUS_OK){
        QMessageBox::information(nullptr,"错误","开启UsbCan失败");
        return -1;
    }
    return 1;
}

int UsbCanUnion::closeUsbCan(){
    if(ZCAN_CloseDevice(dhandle)!=STATUS_OK){
        QMessageBox::information(nullptr,"错误","关闭UsbCan失败");
        return -1;
    }
    return 1;
}



uint UsbCanUnion::ReceiveFromCan(unsigned int channelid){
    // 第二个参数 0=CAN 1=CANFD 2= 合并接收。
    uint receiveNum=ZCAN_GetReceiveNum(chHandle[channelid],0);
    // 返回实际读到的帧数
    return ZCAN_Receive(chHandle[channelid],&receive_data,receiveNum,100);
}
// 发送数据
int UsbCanUnion::SendToCan(uint channelid){
    // 每次操作时都将响应的结构体初始化
    memset(&send_frame, 0, sizeof(send_frame));
    // 使用 MAKE_CAN_ID构造 Can_id
    // 32 bit CAN_ID + EFF/RTR/ERR flags
    // EFF 表示是否为扩展帧  RTR 表示是否为远程帧  ERR 表示是否为错误帧(目前只能为0)

    // frame是can_frame结构体，用来保存数据
    // 可以将can_frame看作为 数据包，包内容及长度可以自己定义
    // 可以将内容都集中到一个数据包发送，也可以分开发送
    send_frame.frame.can_id = MAKE_CAN_ID(0x100, 1, 0, 0);
    send_frame.frame.can_dlc = 8;
    BYTE data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    memcpy(send_frame.frame.data, data, sizeof(data));
    // transmit  通道的句柄 zcan_transmit_data帧数据的首地址 数据帧长度
    if(ZCAN_Transmit(chHandle[channelid],&send_frame,1)==-1){
        QMessageBox::information(nullptr,"错误","发送数据失败");
    }
    return -1;
}

int UsbCanUnion::StartTestMode(){

    BYTE mes[8][8]={
        {0x57, 0x35, 0xB8, 0x37, 0xFF, 0xFF, 0xE0, 0xFD},
        {0x7B, 0x35, 0x67, 0x35, 0xFF, 0xFF, 0xAD, 0xFD},
        {0x87, 0x35, 0x8F, 0x35, 0x7B, 0x35, 0xAD, 0xB5},
        {0x57, 0x35, 0xB8, 0x37, 0xFF, 0xFF, 0xE0, 0xFD},
        {0xB4, 0x35, 0x19, 0x38, 0xFF, 0xFF, 0xE0, 0xFD},
        {0x95, 0x35, 0x95, 0x35, 0xFF, 0xFF, 0xAD, 0xFD},
        {0x91, 0x35, 0x99, 0x35, 0x95, 0x35, 0xAD, 0xB5},
        {0xB4, 0x35, 0x19, 0x38, 0xFF, 0xFF, 0xE0, 0xFD},
    };


    QVector<int> a_objs_id={
                          0x18FD2083,0x14FD3E83,0x18FF7AD3,0x18FDB483,
                          0x18FD2084,0x14FD3E84,0x18FF7AD4,0x18FDB484,
                         };//0x10FD4783,  0x10FD4784
    QVector<int> b_objs_id={
                          0x18FF75D3,0x18FF76D3,0x18FF74D3,0x18FF77D3,
                          0x18FF75D4,0x18FF76D4,0x18FF74D4,0x18FF77D4
                         };//0x10FD47D3,   0x10FD47D4
    QVector<int> objs_id=b_objs_id;
    ULONG len=objs_id.size();
    int size=b_objs_id.size();
//    for(ULONG i=0;i<len;i++){
//        memmove(objs[i].Data,mes[i],8);
//        objs[i].DataLen=8;
//        objs[i].ID=objs_id[i];
//    }
    return len;
}


