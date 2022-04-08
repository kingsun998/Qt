#include <wzserialport.h>
#include <string.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

class WZSerialPort wzserialport_eight(1);
class WZSerialPort wzserialport_single(0);
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
    testState.clear();
    testState.resize(8);
    for (uint i=0;i<8;i++) {
        resistance[i]=0;
        testState[i]=true;
    }
    time=0;
}

EightPortStruct::EightPortStruct():PortStruct (){
    reset();
}
EightPortStruct::~EightPortStruct(){
}

WZSerialPort::WZSerialPort(int state,QObject *parent):QObject(parent)
{

    this->state=state;
    connected=false;
    connect(this,&WZSerialPort::send_receive,this,&WZSerialPort::send_receive_synchronously);
    connect(this,&WZSerialPort::listen_to_port_signal,this,&WZSerialPort::listen_to_port);
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
    qDebug()<<"voltage:"<<QString::fromStdString(spt->voltage);
    offset+=5;
    if(buf[offset]!=-80){
        for(int i=0;i<6;i++){
            spt->resistance+=buf[offset+i];
        }
    }else{
        spt->resistance="PASS";
    }
    qDebug()<<"resistance:"<<QString::fromStdString(spt->resistance);
    offset+=6+6;  //6+6空格
    for(int i=0;i<4;i++){
        spt->time+=buf[offset+i];
    }
    qDebug()<<"time:"<<QString::fromStdString(spt->time);
    offset+=4;
    qDebug()<<uchar(buf[offset]);

    switch(uchar(buf[offset])){
        case 145:
            spt->testState="pass";
        break;
        case 144:
            spt->testState="normal";
        break;
        case 146:
            spt->testState="testfail";
        break;
    }
    qDebug()<<"testState:"<<QString::fromStdString(spt->testState);
}

void Decode_eight_channel(char* buf,int wCount,bool bReadStat,PortStruct* pt){
    if(!bReadStat){
        pt->state=0;
        return ;
    }
    EightPortStruct *ept=(EightPortStruct*)pt;
    if(buf[0]!=10){
        qDebug()<<"帧错误";
        return;
    }
    //读取IEEE 754,并将之转换为int
    byte ary[4];
    byte tmp[4];
    for (uint i=0;i<8;i++) {
        for (int j=0;j<4;j++) {
            ary[j]=ToInt(buf+1+i*4+j);
        }
        for (int j=0;j<4;j++)tmp[3-j]=ary[j];
        ept->resistance[i]=*(float*)tmp;
        ept->testState[i]=buf[1+4*8+1+i*2]==90?true:false;
    }

    ept->time=*(buf+1+4*8);
    qDebug()<<"resistance:"<<ept->resistance[0];
    qDebug()<<"testState:"<<ept->testState[0];
    qDebug()<<"time:"<<ept->time;
}

bool WZSerialPort::receive(void (Decode)(char*,int,bool,PortStruct*))
{
    qDebug()<<"receive "<< int(this->synchronizeflag);
    HANDLE hCom = *(HANDLE*)pHandle;
    char buf[1024];
    DWORD wCount = 1024; //成功读取的数据字节数
    qDebug()<<"phandle"<<pHandle<<" "<<int(this->synchronizeflag);
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


