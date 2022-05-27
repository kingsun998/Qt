#include <wzserialport.h>
#include <string.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <settings.h>

#include <iostream>
#include <sstream>
#include <iomanip>
class WZSerialPort wzserialport_eight(1);
class WZSerialPort wzserialport_single(0);
class WZSerialPort wzserialport_multiple(0);
PortStruct::PortStruct(){
    state=-1;
    testtime="-1";
}
void PortStruct::reset(){
}
PortStruct::~PortStruct(){
}


void SinglePortStruct::reset(){
    voltage="";
    resistance="";
    electric="";
    testState="";
    time="";
}
SinglePortStruct::SinglePortStruct():PortStruct (){
    reset();
}
SinglePortStruct::~SinglePortStruct(){
}


void EightPortStruct::reset(){
    resistance.clear();
    resistance.resize(8);
    voltage.clear();
    voltage.resize(8);
    testState.clear();
    testState.resize(8);
    for (uint i=0;i<8;i++) {
        voltage[i]="";
        resistance[i]="";
        testState[i]=true;
    }
    time=0;
}

EightPortStruct::EightPortStruct():PortStruct (){
    reset();
}
EightPortStruct::~EightPortStruct(){
}

void MultipleProcessStruct::reset(){
    pcdID="";
    highTemperature=0;
    lowTemperature=0;
}
MultipleProcessStruct::MultipleProcessStruct(){
    reset();
}
MultipleProcessStruct::~MultipleProcessStruct(){
}

WZSerialPort::WZSerialPort(int state,QObject *parent):QObject(parent)
{

    this->state=state;
    connected=false;
    connect(this,&WZSerialPort::send_receive,this,&WZSerialPort::send_receive_synchronously);
    connect(this,&WZSerialPort::listen_to_port_signal,this,&WZSerialPort::listen_to_port);

    connect(this,&WZSerialPort::send_multiple_receive,
            this,&WZSerialPort::send_multiple_receive_synchronously);

    this->moveToThread(&thread);
    thread.start();

}

WZSerialPort::~WZSerialPort()
{
    thread.quit();
}

bool WZSerialPort::open(const char* portname,
                        DWORD baudrate,
                        char parity,
                        char databit,
                        char stopbit,
                        char synchronizeflag)
{

    if(connected){
        close();
    }
    //同步方式
    this->synchronizeflag = synchronizeflag;
    qDebug()<<int(synchronizeflag);
    HANDLE hCom = NULL;
    if (this->synchronizeflag)
    {
        //同步方式
        hCom = CreateFileA(portname, //串口名
                                    GENERIC_READ | GENERIC_WRITE, //支持读写
                                    0, //独占方式，串口不支持共享
                                    NULL,//安全属性指针，默认值为NULL
                                    OPEN_EXISTING, //打开现有的串口文件
                                    0, //0：同步方式，FILE_FLAG_OVERLAPPED：异步方式
                                    NULL);//用于复制文件句柄，默认值为NULL，对串口而言该参数必须置为NULL
    }
    else
    {
        //异步方式
        hCom = CreateFileA(portname, //串口名
                                    GENERIC_READ | GENERIC_WRITE, //支持读写
                                    0, //独占方式，串口不支持共享
                                    NULL,//安全属性指针，默认值为NULL
                                    OPEN_EXISTING, //打开现有的串口文件
                                    FILE_FLAG_OVERLAPPED, //0：同步方式，FILE_FLAG_OVERLAPPED：异步方式
                                    NULL);//用于复制文件句柄，默认值为NULL，对串口而言该参数必须置为NULL
    }

    if(hCom == INVALID_HANDLE_VALUE)  //（NULL）-1  不就是-1吗
    {
        return false;
    }

    //配置缓冲区大小
    if(! SetupComm(hCom,1024, 1024))
    {
        return false;
    }

    // 配置参数
    DCB p;
    memset(&p, 0, sizeof(p));
    p.DCBlength = sizeof(p);
    p.BaudRate = baudrate; // 波特率
    p.ByteSize = databit; // 数据位

    switch (parity) //校验位
    {
    case 0:
        p.Parity = NOPARITY; //无校验
        break;
    case 1:
        p.Parity = ODDPARITY; //奇校验
        break;
    case 2:
        p.Parity = EVENPARITY; //偶校验
        break;
    case 3:
        p.Parity = MARKPARITY; //标记校验
        break;
    }

    switch(stopbit) //停止位
    {
    case 1:
        p.StopBits = ONESTOPBIT; //1位停止位
        break;
    case 2:
        p.StopBits = TWOSTOPBITS; //2位停止位
        break;
    case 3:
        p.StopBits = ONE5STOPBITS; //1.5位停止位
        break;
    }
    //handle 和 配置参数
    if(! SetCommState(hCom, &p))
    {
        // 设置参数失败
        return false;
    }

    //超时处理,单位：毫秒
    //总超时＝时间系数×读或写的字符数＋时间常量
    COMMTIMEOUTS TimeOuts;
    TimeOuts.ReadIntervalTimeout = 1000; //读间隔超时
    TimeOuts.ReadTotalTimeoutMultiplier = 500; //读时间系数
    TimeOuts.ReadTotalTimeoutConstant = 5000; //读时间常量
    TimeOuts.WriteTotalTimeoutMultiplier = 500; // 写时间系数
    TimeOuts.WriteTotalTimeoutConstant = 2000; //写时间常量

    //设置时间
    SetCommTimeouts(hCom,&TimeOuts);
    //清空串口缓冲区    purge清楚
    PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);

    memmove(pHandle, &hCom, sizeof(hCom));// 保存句柄,这里是将句柄保存到 pHandle中

    connected=true;
    if(state){
//        timer.stop();
//        timer.setInterval(100);
//        timer.start();
        emit listen_to_port_signal();
    }
    qDebug()<<"打开串口成功";
    return true;
}

void WZSerialPort::close()
{
    if(connected){
        HANDLE hCom = *(HANDLE*)pHandle;
        CloseHandle(hCom);
        connected=false;
    }

}

DWORD WZSerialPort::send(byte* command,int size)
{
    qDebug()<<"当前线程:"<<QThread::currentThreadId();

    HANDLE hCom = *(HANDLE*)pHandle;

    if (this->synchronizeflag)
    {
        // 同步方式
        DWORD dwBytesWrite = size; //成功写入的数据字节数
        BOOL bWriteStat = WriteFile(hCom, //串口句柄
                                    command, //数据首地址
                                    dwBytesWrite, //要发送的数据字节数
                                    &dwBytesWrite, //DWORD*，用来接收返回成功发送的数据字节数
                                    NULL); //NULL为同步发送，OVERLAPPED*为异步发送
        if (!bWriteStat)
        {
            return 0;
        }
        return dwBytesWrite;
    }
    else
    {
        //异步方式
        DWORD dwBytesWrite = size; //成功写入的数据字节数
        DWORD dwErrorFlags; //错误标志
        COMSTAT comStat; //通讯状态
        OVERLAPPED m_osWrite; //异步输入输出结构体

        //创建一个用于OVERLAPPED的事件处理，不会真正用到，但系统要求这么做
        memset(&m_osWrite, 0, sizeof(m_osWrite));
        m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, L"WriteEvent");

        ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误，获得设备当前状态
        BOOL bWriteStat = WriteFile(hCom, //串口句柄
            command, //数据首地址
            dwBytesWrite, //要发送的数据字节数
            &dwBytesWrite, //DWORD*，用来接收返回成功发送的数据字节数
            &m_osWrite); //NULL为同步发送，OVERLAPPED*为异步发送
        //在异步条件下，这里是会返回0的，表示成功
        if (!bWriteStat)
        {
            int LastErrorCode=GetLastError();
            //在异步条件下，这里是会返回0的，表示成功
            if( LastErrorCode == ERROR_SUCCESS)
            {
                return 1;
            }
            else if ( LastErrorCode== ERROR_IO_PENDING) //如果串口正在写入
            {
                WaitForSingleObject(m_osWrite.hEvent, 1000); //等待写入事件1秒钟
                return 1;
            }
            else
            {
                qDebug()<<"unkonw error";
                ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误
                CloseHandle(m_osWrite.hEvent); //关闭并释放hEvent内存
                return 0;
            }
        }else{
            return 0;
        }

    }
}

int ToInt(char* buf){
    int base=1;
    int total=0;
    for(int i=0;i<8;i++){
        total+=base*(*buf>>i&1);
        base*=2;
    }
    return total;
}

void Decode_single_channel(char* buf,int wCount,bool bReadStat,PortStruct* pt){

    if(!bReadStat){
        pt->state=0;
        return ;
    }
    SinglePortStruct *spt=(SinglePortStruct*)pt;
    int offset=wCount-24+1;
    for(int i=0;i<5;i++){
        spt->voltage+=buf[offset+i];
    }

    offset+=5;
    std::string s="";
    for(int i=0;i<6;i++){
        s+=buf[offset+i];
    }

    if(buf[offset]!=-80){
        for(int i=0;i<6;i++){
            spt->resistance+=buf[offset+i];
        }
    }else{
        spt->resistance="Pass";
    }
    offset+=6+6;  //6+6空格
    for(int i=0;i<4;i++){
        spt->time+=buf[offset+i];
    }
    qDebug()<<"time:"<<QString::fromStdString(spt->time);
    offset+=4;

    switch(uchar(buf[offset])){
        case 145:
            spt->testState="Pass";
        break;
        case 144:
            spt->testState="Normal";
        break;
        case 146:
            spt->testState="Error";
            if(spt->resistance=="Pass"){
                spt->resistance="Short";
            }
        break;
    }
    qDebug()<<"testState:"<<QString::fromStdString(spt->testState);
}

static std::string dec2hex(int i) //将int转成16进制字符串
{
    std::stringstream ioss; //定义字符串流
    std::string s_temp; //存放转化后字符
    ioss << setiosflags(std::ios::uppercase) << std::hex << i; //以十六制(大写)形式输出
    ioss >> s_temp;
    return s_temp;
}


void Decode_eight_channel(char* buf,int wCount,bool bReadStat,PortStruct* pt){
    if(!bReadStat){
        pt->state=0;
        return ;
    }
    EightPortStruct *ept=(EightPortStruct*)pt;

    for (int base=0,i=0;i<8;i++) {
        bool flag=false,flag2=false;
        base+=5;
        for (int j=0;j<5;j++) {
            ept->voltage[i]+=buf[base];
            base++;
        }
        ept->voltage[i]+="V";

        if(uint8_t(buf[base])==0xB0){
            flag2=true;
            base+=5;
        }
        else{
            for (int j=0;j<5;j++) {
                ept->resistance[i]+=buf[base];
                base++;
            }
            switch (uint8_t(buf[base])) {
                case 0x47: ept->resistance[i]+="GΩ";break;
                case 0x4D: ept->resistance[i]+="MΩ";break;
                case 0x6B: ept->resistance[i]+="KΩ";flag=true;break;
                case 0x54: ept->resistance[i]+="TΩ";break;
            }
        }

        base+=11;

        switch(uint8_t(buf[base])) {
            case 0x91:
                if (flag2){
                    ept->resistance[i]+="Pass";
                 }
                 ept->testState[i]=true;
            break;
            case 0x92:
                if (flag2){
                    ept->resistance[i]+="0Ω";
                }
                ept->testState[i]=false;
                 break;
        }
        if(flag){
            ept->testState[i]=false;
        }
        base+=2;

    }
    settings.eightChannelFrameMes="";
    for (int i=0;i<240;i++) {
        settings.eightChannelFrameMes+=dec2hex(uint8_t(buf[i]))+" ";
    }
}
void Decode_multiple(char* buf,int wCount,bool bReadStat,PortStruct* pt){
    if(!bReadStat){
        pt->state=0;
        return ;
    }
    MultipleProcessStruct *mpt=(MultipleProcessStruct*)pt;
    std::string s="";
    for (int i=0;i<wCount;i++) {
        s+=dec2hex(ToInt(&buf[i]))+" ";
    }
    qDebug()<<QString::fromStdString(s);
//    for (int base=0,i=0;i<8;i++) {
//        bool flag=false,flag2=false;
//        base+=5;
//        for (int j=0;j<5;j++) {
//            mpt->voltage[i]+=buf[base];
//            base++;
//        }
//        ept->voltage[i]+="V";

//        if(uint8_t(buf[base])==0xB0){
//            flag2=true;
//            base+=5;
//        }
//        else{
//            for (int j=0;j<5;j++) {
//                ept->resistance[i]+=buf[base];
//                base++;
//            }
//            switch (uint8_t(buf[base])) {
//                case 0x47: ept->resistance[i]+="GΩ";break;
//                case 0x4D: ept->resistance[i]+="MΩ";break;
//                case 0x6B: ept->resistance[i]+="KΩ";flag=true;break;
//                case 0x54: ept->resistance[i]+="TΩ";break;
//            }
//        }

//        base+=11;

//        switch(uint8_t(buf[base])) {
//            case 0x91:
//                if (flag2){
//                    ept->resistance[i]+="Pass";
//                 }
//                 ept->testState[i]=true;
//            break;
//            case 0x92:
//                if (flag2){
//                    ept->resistance[i]+="0Ω";
//                }
//                ept->testState[i]=false;
//                 break;
//        }
//        if(flag){
//            ept->testState[i]=false;
//        }
//        base+=2;

//    }

}
bool WZSerialPort::receive(void (Decode)(char*,int,bool,PortStruct*))
{
    qDebug()<<"receive "<< int(this->synchronizeflag);
    HANDLE hCom = *(HANDLE*)pHandle;
    char buf[1024];
    DWORD wCount = 1024; //成功读取的数据字节数
    if (this->synchronizeflag)
    {
        //同步方式
        BOOL bReadStat = ReadFile(hCom, //串口句柄
            buf, //数据首地址
            wCount, //要读取的数据最大字节数
            &wCount, //DWORD*,用来接收返回成功读取的数据字节数
            0); //NULL为同步发送，OVERLAPPED*为异步发送
        qDebug()<<"wCount:"<<wCount;
        if(!wCount){
            return false;
        }
        pt->reset();
        Decode(buf,wCount,bReadStat,pt);
        return true;
    }
    else
    {
        //异步方式

        DWORD dwErrorFlags; //错误标志
        COMSTAT comStat; //通讯状态
        OVERLAPPED m_osRead; //异步输入输出结构体

        //创建一个用于OVERLAPPED的事件处理，不会真正用到，但系统要求这么做
        memset(&m_osRead, 0, sizeof(m_osRead));
        m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, L"ReadEvent");

        ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误，获得设备当前状态
        qDebug()<<"缓冲区大小："<<comStat.cbInQue;

        BOOL bReadStat = ReadFile(hCom, //串口句柄
            buf, //数据首地址
            wCount, //要读取的数据最大字节数
            &wCount, //DWORD*,用来接收返回成功读取的数据字节数
            &m_osRead); //NULL为同步发送，OVERLAPPED*为异步发送
        if (!bReadStat)
        {
            int LastErrorCode=GetLastError();
            if (LastErrorCode == ERROR_SUCCESS){
                //这是成功的
            }
            else if (LastErrorCode == ERROR_IO_PENDING) //如果串口正在读取中
            {
                //GetOverlappedResult函数的最后一个参数设为TRUE
                //函数会一直等待，直到读操作完成或由于错误而返回
                GetOverlappedResult(hCom, &m_osRead, &wCount, TRUE);
            }
            else
            {
                ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误
                CloseHandle(m_osRead.hEvent); //关闭并释放hEvent的内存
                pt->state=0;
            }
        }else{
            pt->state=0;
        }
        if(!wCount){
            return false;
        }
        Decode(buf,wCount,!bReadStat,pt);
        return true;
    }
}


void WZSerialPort::send_receive_synchronously(){

    byte command[3]={170,250,191};
    pt->reset();

    DWORD writenums=send(command,3);
    qDebug()<<"Writenums:"<<writenums;
    if(!writenums){
        QMessageBox::critical(nullptr,"错误","发送失败");
        return ;
    }
    if(receive(Decode_single_channel)){
        qDebug()<<"emit single channel";
        emit ReceiveSinglePortMes();
    }
}

uint crc16(byte *ptr, byte len)//ptr:需要校验的数据寄存器；len:需要校验的位数；
{
    uint crc = 0xffff;
    byte kc = 0;
    byte kn = 0;
    byte i = 0;
    while (len-- != 0)
    {
        crc = (crc & 0xff00) + (((crc & 0xff) ^ (ptr[i])) & 0xff);
        for (kn = 0; kn < 8; kn++)
        {
            kc = (byte)(crc & 1);
            crc >>= 1;
            if (kc != 0) crc ^= 0xa001;
        }
        i++;
    }
    return crc;
}

void WZSerialPort::send_multiple_receive_synchronously(){
    byte command[6]={0x01, 0x67, 0x04, 0x00,0,0};
    uint crc=crc16(command,4);
    qDebug()<<"crc value:"<<crc;
    command[5]=crc%256;  //f0
    command[6]=crc/256;  //55
    qDebug()<<"crc low:"<<crc%256;
    qDebug()<<"crc high:"<<crc/256;
    pt->reset();

    DWORD writenums=send(command,6);
    qDebug()<<"Writenums:"<<writenums;
    if(!writenums){
        QMessageBox::critical(nullptr,"错误","发送失败");
        return ;
    }
    if(receive(Decode_multiple)){
        qDebug()<<"emit single channel";
        emit ReceiveSinglePortMes();
    }
}

//不停的监听串口
void WZSerialPort::listen_to_port(){
    qDebug()<<"sub thread:"<<QThread::currentThreadId();
    bool fg=receive(Decode_eight_channel);
    qDebug()<<"fg:"<<fg;
    if(fg){
        emit ReceiveEightPortMes();
    }

    emit listen_to_port_signal();
}

std::string Wchar_tToString(wchar_t *wchar)
{
    wchar_t * wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte的运用
    char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
    psText = new char[dwNum];
    WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte的再次运用
    std::string szDst = psText;// std::string赋值
    delete []psText;
    return szDst;
}

std::vector<std::string> WZSerialPort::getComPort(){
    HKEY hKey;
    std::vector<std::string> comName;
    wchar_t portName[256], commName[256];
    //打开串口注册表对应的键值
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey))
    {
        int i = 0;
        DWORD  dwLong, dwSize;
        while (TRUE)
        {

            dwLong = dwSize = sizeof(portName);
            //枚举串口
            if (ERROR_NO_MORE_ITEMS == ::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize))
            {
                break;
            }
            comName.push_back(Wchar_tToString(commName));
            i++;
        }
        //关闭注册表
        RegCloseKey(hKey);
    }
    else
    {
        QMessageBox::critical(nullptr,"警告","您的计算机的注册表上没有HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm项");
    }
    //返回串口号
    return comName;
}

QStringList WZSerialPort::get_available_list()
{
    QStringList list_avail_sp;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            list_avail_sp.append(serial.portName());
            serial.close();
        }
    }

    return list_avail_sp;
}

void WZSerialPort::setPortStruct(PortStruct *pt){
    this->pt=pt;
}


