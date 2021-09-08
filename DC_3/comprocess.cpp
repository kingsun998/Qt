#include "comprocess.h"
#include "QMessageBox"
#include "process.h"
#include <QDebug>
#include <qstringlist.h>
#include <settings.h>

class ComProcess comprocess;

void setCheckSum(unsigned char *array,int arraylen){
    int total=0;
    QString s,s2;
    s+=QString::number(array[0],16)+" ";
    for (auto i=1;i<=arraylen-3;i++) {
        total+=array[i];
        s+=QString::number(array[i],16)+" ";
        s2+=QString::number(array[i],16)+" ";
    }
//    qDebug()<<total;
    //先把数字限制在256之内
    array[arraylen-2]=settings.ox_ascii[total%256/16];
    array[arraylen-1]=settings.ox_ascii[total%16];
    s+=QString::number(settings.ox_ascii[total%256/16],16)+" ";
    s+=QString::number(settings.ox_ascii[total%16],16)+" ";
    qDebug()<<"final: "<<s;
}

Q_DECLARE_METATYPE(ComParameter)
Q_DECLARE_METATYPE(Message)
ComProcess::ComProcess(QObject *parent)
{
    qRegisterMetaType<ComParameter>("ComParameter");
    qRegisterMetaType<Message>("Message");

    //一个寄存器两个字节，所以 字节个数=寄存器个数*2
    /***
     * D20->D23 20*2+4096=4136->1028->0x31,0x30,0x32,0x38 4个寄存器 读8个字节->08(16进制)->0x30,0x38。
     * **/
    unsigned char D20_D23[]={0x02,0x30,0x31,0x30,0x32,0x38,0x30,0x38,0x03,0,0};
    memcpy(this->D20_D23,D20_D23,sizeof(D20_D23));
    setCheckSum(this->D20_D23,11);
    /***
     * D380->D394 380*2+4096=4856->12f8->0x31,0x32,0x46,0x38 15个寄存器 读30个字节->1e(16进制)->0x31,0x45
     * **/
    unsigned char D380_D393[]={0x02,0x30,0x31,0x32,0x46,0x38,0x31,0x45,0x03,0,0};
    memcpy(this->D380_D393,D380_D393,sizeof(D380_D393));
    setCheckSum(this->D380_D393,11);
    /***
     * D422->D446 422*2+4096=4940->134c->0x31,0x33,0x34,0x43 25个寄存器 读50个字节->32(16进制)->0x33,0x32
     * **/
    unsigned char D422_D446[]={0x02,0x30,0x31,0x33,0x34,0x43,0x33,0x32,0x03,0,0};
    memcpy(this->D422_D446,D422_D446,sizeof(D422_D446));
    setCheckSum(this->D422_D446,11);
    /***
     * D690->D724 690*2+4096=5476->1564->0x31,0x35,0x36,0x34 35个寄存器 读70个字节->46(16进制)->0x34,0x36
     * **/
    unsigned char D690_D725[]={0x02,0x30,0x31,0x35,0x36,0x34,0x34,0x36,0x03,0,0};
    memcpy(this->D690_D725,D690_D725,sizeof(D690_D725));
    setCheckSum(this->D690_D725,11);
    /***
     * D1123->D1162 1123*2+4096=6342->18c6->0x31,0x38,0x43,0x36 40个寄存器 读80个字节->50(16进制)->0x35,0x30
     * **/
    unsigned char D1123_D1160[]={0x02,0x30,0x31,0x38,0x43,0x36,0x35,0x30,0x03,0,0};
    memcpy(this->D1123_D1160,D1123_D1160,sizeof(D1123_D1160));
    setCheckSum(this->D1123_D1160,11);

    timer.setInterval(settings.send_frame_interval);
    service.moveToThread(&comthread);
    comthread.start();

    //QT中多线程需要使用
    connect(&timer,&QTimer::timeout,this,&ComProcess::handleTimeOut);
    connect(&service,&ComService::callback,this,&ComProcess::getResult);
    //发送请求信息
    connect(this,&ComProcess::sendRequestCommand,&service,&ComService::sendRequest);
    //发送参数信息
    connect(this,&ComProcess::sendParameterCommand,&service,&ComService::sendParameter);
    //初始化信号
    connect(this,&ComProcess::setParameterCommand,&service,&ComService::setParameter);

    ifsending=false;
    qDebug()<<"current threadid:"<<QThread::currentThreadId();
}

ComProcess::~ComProcess(){
    comthread.quit();
    comthread.wait();
}

void ComProcess::init(ComParameter& comparameter){
    emit setParameterCommand(comparameter);
}


ComService* ComProcess::getService(){
    return &service;
}

void ComProcess::startComProcessTimer(){
    /***
     * 向寄存器写入开始
     * M02 0102->0x30,0x31,0x30,0x32 写入1个寄存器 2个字节
     * 在 9位写入 0x31 ->0001
     * **/

    unsigned char M02[15]={0x02,0x31,0x30,0x31,0x30,0x32,0x30,0x32,0x30,0x31,0x30,0x30,0x03,0,0};
    setCheckSum(M02,15);
    qDebug()<<"comprocess: "<<QThread::currentThreadId();
    emit sendParameterCommand(M02,15,-1);
    qDebug()<<"timer start";
    timer.start();
    ifsending=true;
}

void ComProcess::stopComProcessTimer(){
    //向寄存器写入停止
    unsigned char M03[15]={0x02,0x31,0x30,0x31,0x30,0x33,0x30,0x32,0x30,0x31,0x30,0x30,0x03,0,0};
    setCheckSum(M03,15);
    emit sendParameterCommand(M03,15,-1);
    timer.stop();
    ifsending=false;
    qDebug()<<"timer stop";
}


void ComProcess::handleTimeOut(){
    if(ifsending==false){
        return;
    }
    qDebug()<<"请求数据";
    emit sendRequestCommand(D20_D23,COMMAND,0);
//    emit sendParameterCommand(D380_D393,COMMAND,1);
//    emit sendParameterCommand(D422_D446,COMMAND,2);
//    emit sendParameterCommand(D690_D725,COMMAND,3);
//    emit sendParameterCommand(D1123_D1160,COMMAND,4);

}

long ResultToNum(QVector<unsigned char>& v,int index){
    //低高位，低低位，高高位，高低位
    long result[4]={0};
    for (auto i=0;i<4;i++) {
        if(v[index+i]>='A'){
            result[i]=v[index+i]-55;
        }else{
            result[i]=v[index+i]-48;
        }
    }
    return (result[1]+16*result[0])+256*(result[3]+16*result[2]);
}

void ComProcess::getResult(QVariant var,int arraytype){
    switch (arraytype) {
        case 0:
            emit sendRequestCommand(D380_D393,COMMAND,1);
        break;
        case 1:
            emit sendRequestCommand(D422_D446,COMMAND,2);
        break;
        case 2:
            emit sendRequestCommand(D690_D725,COMMAND,3);
        break;
        case 3:
            emit sendRequestCommand(D1123_D1160,COMMAND,4);
        break;
        //这里接受需要的数组长度为发送的请求长度的两倍
    }
    QVector<unsigned char> vector=var.value<QVector<unsigned char>>();
    QVector<long> vec;
    switch (arraytype) {
        case 0:

            vec.push_back(ResultToNum(vector,1));
            vec.push_back(ResultToNum(vector,5));
            vec.push_back(ResultToNum(vector,9)+256*ResultToNum(vector,13));
//            qDebug()<<vector[9]<<"  "<<vector[10]<<"  "<<vector[11]<<"  "<<vector[12];
//            qDebug()<<ResultToNum(vector,9)+256*ResultToNum(vector,13);
        break;
        case 1:
            vec.push_back(ResultToNum(vector,1));
            vec.push_back(ResultToNum(vector,48)+256*ResultToNum(vector,53));
        break;
        case 2:
            vec.push_back(ResultToNum(vector,1));
            vec.push_back(ResultToNum(vector,41));
            vec.push_back(ResultToNum(vector,73));
            vec.push_back(ResultToNum(vector,81));
            qDebug()<<vector[81]<<"  "<<vector[82]<<"  "<<vector[83]<<"  "<<vector[84];
            qDebug()<<"数组长度xx: "<<ResultToNum(vector,81);
        break;
        case 3:
            vec.push_back(ResultToNum(vector,1));
            vec.push_back(ResultToNum(vector,41));
            vec.push_back(ResultToNum(vector,81));
            vec.push_back(ResultToNum(vector,121));
            vec.push_back(ResultToNum(vector,129));
        break;
        case 4:
            vec.push_back(ResultToNum(vector,1));
            vec.push_back(ResultToNum(vector,41));
            vec.push_back(ResultToNum(vector,81));
            vec.push_back(ResultToNum(vector,121));
            vec.push_back(ResultToNum(vector,149));
        break;
    }

    QVariant newvars=QVariant::fromValue(vec);
    emit returnResult(newvars,arraytype);
}


void ComProcess::sendComParameter(Message& mes){
    /***
     * D22->D23 22*2+4096=4140->102c->0x31,0x30,0x32,0x43 2个寄存器 读4个字节->04(16进制)->0x30,0x34。
     * **/
    long totaltest=mes.totalTestPlc;
    unsigned char test[19]={0x02,0x31,0x31,0x30,0x32,0x43,0x30,0x34,0,0,0,0,0,0,0,0,0x03,0,0};
    test[14]=settings.ox_ascii[totaltest/268435456];
    test[15]=settings.ox_ascii[totaltest%268435456/16777216];
    test[12]=settings.ox_ascii[totaltest%16777216/1048576];
    test[13]=settings.ox_ascii[totaltest%1048576/65536];
    test[10]=settings.ox_ascii[totaltest%65536/4096];
    test[11]=settings.ox_ascii[totaltest%4096/256];
    test[8]=settings.ox_ascii[totaltest%256/16];
    test[9]=settings.ox_ascii[totaltest%16];
    setCheckSum(test,19);
    emit sendParameterCommand(test,19,-1);

    //想了一下，service中使用了临界区，所以可以不用人为控制发送流程。上面代码人为控制了流程
    /***
     *  D322 322*2+4096=4740->1284 0x31,0x32,0x38,0x34 写一个寄存器 2个字节->02->0x30,0x32
     * **/
    int heatPlc=mes.heatPlc;
    if(heatPlc>=0){
        unsigned char D322[]={0x02,0x31,0x31,0x32,0x38,0x34,0x30,0x32,0,0,0,0,0x03,0,0};
        D322[10]=settings.ox_ascii[heatPlc/4096];
        D322[11]=settings.ox_ascii[heatPlc%4096/256];
        D322[8]=settings.ox_ascii[heatPlc%256/16];
        D322[9]=settings.ox_ascii[heatPlc%16];
        setCheckSum(D322,15);
        emit sendParameterCommand(D322,15,-1);
    }

    /***
     * D222 222*2+4096=4540->11bc 0x31,0x31,0x42,0x43 写一个寄存器 2个字节->02->0x30,0x32
     * **/
    int coldPlc=mes.coldPlc;
    if(coldPlc>=0){
        unsigned char D222[]={0x02,0x31,0x31,0x31,0x42,0x43,0x30,0x32,0,0,0,0,0x03,0,0};
        D222[10]=settings.ox_ascii[coldPlc/4096];
        D222[11]=settings.ox_ascii[coldPlc%4096/256];
        D222[8]=settings.ox_ascii[coldPlc%256/16];
        D222[9]=settings.ox_ascii[coldPlc%16];
        setCheckSum(D222,15);
        emit sendParameterCommand(D222,15,-1);
    }

    /***
     * D324-D325 324*2+4096=4744->1288 0x31,0x32,0x38,0x38 写两个寄存器 4个字节->04->0x30,0x34
     * **/
    long totalTestPlc=mes.totalTestPlc;

    if (totalTestPlc>=0){
        unsigned char D324[19]={0x02,0x31,0x31,0x32,0x38,0x38,0x30,0x34,0,0,0,0,0,0,0,0,0x03,0,0};
        D324[14]=settings.ox_ascii[totalTestPlc/268435456];
        D324[15]=settings.ox_ascii[totalTestPlc%268435456/16777216];
        D324[12]=settings.ox_ascii[totalTestPlc%16777216/1048576];
        D324[13]=settings.ox_ascii[totalTestPlc%1048576/65536];
        D324[10]=settings.ox_ascii[totalTestPlc%65536/4096];
        D324[11]=settings.ox_ascii[totalTestPlc%4096/256];
        D324[8]=settings.ox_ascii[totalTestPlc%256/16];
        D324[9]=settings.ox_ascii[totalTestPlc%16];
        setCheckSum(D324,19);
        emit sendParameterCommand(D324,19,-1);
    }
    /***
     * D105 105*2+4096=4306->10d2 0x31,0x30,0x44,0x32 写一个寄存器 2个字节->02->0x30,0x32
     * **/
    int hour=mes.hourPlc;
    if(hour>=0){
        unsigned char D105[15]={0x02,0x31,0x31,0x30,0x44,0x32,0x30,0x32,0,0,0,0,0x03,0,0};
        D105[10]=settings.ox_ascii[hour/4096];
        D105[11]=settings.ox_ascii[hour%4096/256];
        D105[8]=settings.ox_ascii[hour%256/16];
        D105[9]=settings.ox_ascii[hour%16];
        setCheckSum(D105,15);
        emit sendParameterCommand(D105,15,-1);
    }

    /***
     * D106 106*2+4096=4308->10d4 0x31,0x30,0x44,0x34 写一个寄存器 2个字节->02->0x30,0x32
     * **/
    int minutes=mes.minutePlc;
    if(minutes>=0){
        unsigned char D106[15]={0x02,0x31,0x31,0x30,0x44,0x34,0x30,0x32,0,0,0,0,0x03,0,0};
        D106[10]=settings.ox_ascii[minutes/4096];
        D106[11]=settings.ox_ascii[minutes%4096/256];
        D106[8]=settings.ox_ascii[minutes%256/16];
        D106[9]=settings.ox_ascii[minutes%16];
        setCheckSum(D106,15);
        emit sendParameterCommand(D106,15,-1);
    }
    /***
     * D107 107*2+4096=4310->10d6 0x31,0x30,0x44,0x36 写一个寄存器 2个字节->02->0x30,0x32
     * **/
    int seconds=mes.secondPlc;
    if(seconds>=0){
        unsigned char D107[15]={0x02,0x31,0x31,0x30,0x44,0x36,0x30,0x32,0,0,0,0,0x03,0,0};
        D107[10]=settings.ox_ascii[seconds/4096];
        D107[11]=settings.ox_ascii[seconds%4096/256];
        D107[8]=settings.ox_ascii[seconds%256/16];
        D107[9]=settings.ox_ascii[seconds%16];
        setCheckSum(D107,15);
        emit sendParameterCommand(D107,15,-1);
    }


    /***
     * M04 0104->0x30,0x31,0x30,0x34 写入1个寄存器 2个字节
     * **/
    int upheatpipemovedown=mes.upHeatPipeMoveDown;
    if(upheatpipemovedown>=0){
        unsigned char M104[15]={0x02,0x31,0x30,0x31,0x30,0x34,0x30,0x32,0,0,0,0,0x03,0,0};
        M104[9]=settings.ox_ascii[upheatpipemovedown];
        setCheckSum(M104,15);
        emit sendParameterCommand(M104,15,-1);
    }
    /***
     * M05 0105->0x30,0x31,0x30,0x35 写入1个寄存器 2个字节
     * **/
    int leftheatpipmovefroward=mes.leftHeatPipeMoveForward;
    if(leftheatpipmovefroward>=0){
        unsigned char M105[15]={0x02,0x31,0x30,0x31,0x30,0x35,0x30,0x32,0,0,0,0,0x03,0,0};
        M105[9]=settings.ox_ascii[leftheatpipmovefroward];
        setCheckSum(M105,15);
        emit sendParameterCommand(M105,15,-1);
    }

    /***
     * M06 0106->0x30,0x31,0x30,0x36 写入1个寄存器 2个字节
     * **/

    int changeChannle=mes.changeChannelPlc;
    if(changeChannle>=0){
        unsigned char M106[15]={0x02,0x31,0x30,0x31,0x30,0x36,0x30,0x32,0,0,0,0,0x03,0,0};
        M106[9]=settings.ox_ascii[changeChannle];
        setCheckSum(M106,15);
        emit sendParameterCommand(M106,15,-1);
    }
}

