#ifndef WZSERIALPORT_H
#define WZSERIALPORT_H

#include <QObject>
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <QThread>
#include <vector>
#include <string>
#include <QThread>
#include <QTimer>
#include <vector>
/*
 *
    作者：欧阳伟
    日期：2017-12-14
    类名：WZSerialPort
    用途：串口读写
    示例：
        WZSerialPort w;
        if(w.open("COM1",9600，0，8，1))
        {
            w.send("helloworld",10);
            char buf[1024];
            w.receive(buf,1024);
        }
     改写
*/
extern class WZSerialPort wzserialport_eight;
extern class WZSerialPort wzserialport_single;
class PortStruct
{
public:
    int state;
    QString testtime;
    PortStruct();
    virtual ~PortStruct();
    virtual void reset();
};

class SinglePortStruct: public PortStruct
{
public:
    std::string voltage;
    std::string resistance;
    std::string electric;
    std::string testState;
    std::string time;
    SinglePortStruct();
    virtual ~SinglePortStruct();
    void reset();
};

class EightPortStruct: public PortStruct
{
public:
    std::vector<double> resistance;
    std::vector<bool> testState;
    int time;
    EightPortStruct();
    virtual ~EightPortStruct();
    void reset();
};

class WZSerialPort:public QObject
{
    Q_OBJECT
public:
    WZSerialPort(int state,QObject *parent=nullptr);
    ~WZSerialPort();

    // 打开串口,成功返回true，失败返回false
    // portname(串口名): 在Windows下是"COM1""COM2"等，在Linux下是"/dev/ttyS1"等
    // baudrate(波特率): 9600、19200、38400、43000、56000、57600、115200
    // parity(校验位): 0为无校验，1为奇校验，2为偶校验，3为标记校验
    // databit(数据位): 4-8，通常为8位
    // stopbit(停止位): 1为1位停止位，2为2位停止位,3为1.5位停止位
    // synchronizable(同步、异步): 0为异步，1为同步
    bool open(const char* portname, DWORD baudrate=2400, char parity=0, char databit=8, char stopbit=1, char synchronizeflag=0);

    //关闭串口，参数待定
    void close();

    std::vector<std::string> getComPort();

    QStringList get_available_list();

    void setPortStruct(PortStruct* pt);
private:
    //发送数据或写数据，成功返回发送数据长度，失败返回0
    DWORD send(byte* command,int size);
    //接受数据或读数据，成功返回读取实际数据的长度，失败返回0
    bool receive(void (Decode)(char*,int,bool,PortStruct*));

signals:
    void send_receive();
    void ReceiveSinglePortMes();
    void ReceiveEightPortMes();
    void listen_to_port_signal();
private slots:
    void send_receive_synchronously();
    void listen_to_port();
private:
    int pHandle[16];
    char synchronizeflag;
    QThread thread;
    QTimer timer;
    bool connected;
    PortStruct *pt;

    int state;
};


//void ConvertDexToIEE754(float fpointer,unsigned char *a)  //十进制转化为 IEEE745 小数
//{
//    int Flag=0;
//    double   integer,decimal;    //整数，小数
//    unsigned  long   bininteger,bindecimal;   //二进制整数，二进制小数
//    int   _power,i;

//    if(fpointer<0)
//    {
//      fpointer=fpointer*(-1);
//      Flag=1;
//    }

//    decimal = modf(fpointer,&integer);   //将整数位到存到 integer ，小数位返回到decimal
//    if(decimal || integer)    //判断 fpointer是否为0
//    {
//     bindecimal = (unsigned  long )(decimal * 0x800000);   //0x800000=2^23 。得到小数位二进制表现形式
//     while((bindecimal & 0xff800000) > 0) //计算有没有超过23位二进制数
//     bindecimal >>= 1;
//     if(integer > 0)
//     {
//      bininteger = (unsigned  long )integer;
//       for(i=0;i<32;i++)               //计算整数部分的2的幂指数，整数位转化为二进制后的位数，计算小数点移动的位数
//       {
//          if(bininteger&0x1)
//         _power = i;
//         bininteger >>= 0x1;
//       }
//    bininteger = (unsigned  long )integer;
//    bininteger &= ~(0x1 << _power); //去掉最高位的1
//    if(_power >= 23) //如果幂指数>23 则舍弃小数位部分
//    {
//          bininteger >>= (_power-23);
//          bindecimal = 127+_power;
//          bininteger |= bindecimal << 23;
//    }
//    else
//    {
//          bininteger <<= (23 - _power);//将去掉最高位的整数二进制，移至第22为
//          bindecimal >>= _power;       //将小数部分左移1，
//          bininteger |= bindecimal;    //二者向或得到1.x的x，
//          bindecimal = 127+_power;     //指数，右移的小数点数+127
//          bininteger |= bindecimal << 23; // 指数为右移23为变为或上x。
//    }
//  }
//    else if(integer == 0)
//    {
//            bindecimal <<= 9; //将小数部分的二进制移至最高位31位
//            _power = 0;
//            bininteger = bindecimal;
//            while(bininteger == ((bindecimal<<1)>>1)) //判断小数位最高位是否为1.  最高位为0 ：真
//            {
//              _power++;
//              bindecimal <<= 0x1;
//              bininteger = bindecimal;  //直到最高位为1,退出循环
//            }
//            _power++;
//            bindecimal <<= 0x1;    //将1.x的1去掉 求x的值，
//            bindecimal >>= 9;      //将小数位回到0-22位
//            bininteger = bindecimal; //暂存到二进制整数中，
//            bindecimal = 127-_power;
//            bininteger |= bindecimal << 23;  //将指数为右移值23为向或得到其值，
//    }
//     if(Flag==1)
//      bininteger |= 0x80000000;


//    i = 0;
//    a[i++] =(unsigned char) ((bininteger >> 24) & 0xff);
//    a[i++] = (unsigned char)((bininteger >> 16) & 0xff);
//    a[i++] = (unsigned char)((bininteger >> 8 ) & 0xff);
//    a[i++] =(unsigned char)( bininteger & 0xff);
//    }
//}

//float ConvertIEE754ToDex(unsigned char *SpModRegister)
//{
//float x,fpointer;
//unsigned long bininteger,bintmp;
//int _power,i=0,s;

//bintmp = SpModRegister[i++] << 8;
//bintmp |= SpModRegister[i++];
//bininteger = SpModRegister[i++] << 8;
//bininteger |= SpModRegister[i++];
//bininteger |= bintmp << 16;

//if(bininteger == 0)
//return 0.0;

//bintmp = bininteger & 0x7FFFFF;
//x = (double)bintmp / 0x800000;
//bintmp = bininteger >> 23;
//_power = bintmp & 0xff;

//bintmp = bininteger & 0x80000000;
//s = (bintmp) ? 1 : 0 ;

//bintmp =(unsigned  long ) pow(2,fabs(_power-127)) ;
//if(_power >= 127)
//fpointer = (1+x) * bintmp ;
//else
//fpointer = (1+x) / bintmp ;
////bintmp = (fpointer * 10000 + 5)/10;
////fpointer = (double)bintmp / 1000;

// if(s)
//   fpointer *= -1;

// return (fpointer);
//}


#endif  // WZSERIALPORT_H
