﻿#include "standarset.h"
#include "ui_standarset.h"
#include <qdebug.h>
#include <vector>
#include <settings.h>

#include <iostream>
#include <sstream>
#include <iomanip>

standarSet::standarSet(QWidget *parent):
    ui(new Ui::standarset_ui)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&standarSet::click);
}


static std::string dec2hex(int i) //将int转成16进制字符串
{
    std::stringstream ioss; //定义字符串流
    std::string s_temp; //存放转化后字符
    ioss << setiosflags(std::ios::uppercase) << std::hex << i; //以十六制(大写)形式输出
    //ioss << resetiosflags(ios::uppercase) << hex << i; //以十六制(小写)形式输出//取消大写的设置
    ioss >> s_temp;
    return s_temp;
}


void standarSet::click(bool checked)
{
//    QStringList filenames={"C:/Users/Administrator/Desktop/Qt_master/DC_3_Qt_5_12_4_MinGW_32_Release/release/savefiles/temptestsB/2022_01_21.xls",
//    "C:/Users/Administrator/Desktop/Qt_master/DC_3_Qt_5_12_4_MinGW_32_Release/release/savefiles/temptestsA/2022_01_21.xls"};

//    QXlsx::Document doc2("C:/Users/Administrator/Desktop/Qt_master/DC_3_Qt_5_12_4_MinGW_32_Release/release/savefiles/testA/2022_01_21.xls");


//    doc2.addSheet("1");
//    doc2.selectSheet("1");

//    qDebug()<<"start merge";

//    long line=1;
//    //这里是 counts-1 的原因为：需要控制数量，这样可以避免对当前正在追加保存的文件进行操作
//    for (long i=0;i<filenames.size();i++) {
//        QXlsx::Document doc(filenames[i]);
//        doc.selectSheet(QString::number(doc.sheetNames().size()));
//        qDebug()<<"current_file:"<<filenames[i];
//        QXlsx::CellRange range=doc.dimension();
//        long rownums=range.rowCount();
//        long colnums=range.columnCount();
//        long row=(i==0?1:2);
//        for (;row<=rownums;row++) {
//            for (int col=1;col<=colnums;col++) {
//                doc2.write(line,col,doc.read(row,col));
//            }
//            line++;
//        }
//        qDebug()<<"Write "<<filenames[i]<<" finished.";
//    }
//    doc2.save();
//    qDebug()<<"save finish";

//    int buf[1024]={ 0xAA,0x01,0xFC,0xBF,0xAB,0x34,0x39,0x39,0x2E,0x33,0x32,0x30,0x2E,0x31,0x30,0x4D,0x32,0x34,0x2E,
//                    0x38,0x34,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,0xAA,0x02,0xFC,0xBF,0xAB,0x34,0x39,0x35,0x2E,0x39,
//                    0x33,0x2E,0x33,0x38,0x39,0x4D,0x31,0x34,0x36,0x2E,0x33,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,0xAA,
//                    0x03,0xFC,0xBF,0xAB,0x34,0x39,0x37,0x2E,0x33,0x33,0x2E,0x32,0x37,0x33,0x4D,0x31,0x35,0x31,0x2E,
//                    0x39,0x75,0x20,0x35,0x35,0x31,0x91,0xAF,0xAA,0x04,0xFC,0xBF,0xAB,0x34,0x39,0x35,0x2E,0x39,0x31,
//                    0x32,0x2E,0x37,0x35,0x4D,0x33,0x38,0x2E,0x38,0x38,0x75,0x20,0x35,0x35,0x32,0x91,0xAF,0xAA,0x05,
//                    0xFC,0xBF,0xAB,0x35,0x30,0x30,0x2E,0x30,0x38,0x2E,0x36,0x38,0x37,0x4D,0x35,0x37,0x2E,0x35,0x35,
//                    0x75,0x20,0x35,0x35,0x34,0x91,0xAF,0xAA,0x06,0xFC,0xBF,0xAB,0x35,0x30,0x38,0x2E,0x36,0x31,0x35,
//                    0x2E,0x39,0x37,0x4D,0x33,0x31,0x2E,0x38,0x34,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,0xAA,0x07,0xFC,
//                    0xBF,0xAB,0x34,0x39,0x39,0x2E,0x33,0x39,0x2E,0x30,0x39,0x35,0x4D,0x35,0x34,0x2E,0x39,0x30,0x75,
//                    0x20,0x35,0x35,0x32,0x91,0xAF,0xAA,0x08,0xFC,0xBF,0xAB,0x34,0x39,0x39,0x2E,0x39,0x31,0x35,0x2E,
//                    0x31,0x35,0x4D,0x33,0x32,0x2E,0x39,0x39,0x75,0x20,0x35,0x35,0x31,0x91,0xAF,0xCD,0xA0,0x33,0x53,
//                    0xCD,0x4C,0x33,0x2B,0x9A,0x09,0x66,0x2E,0x00,0x10,0x9A,0x21};
//                 AA 6 FC BF AB 35 30 39 2E 33 B0 B0 B0 B0 B0 54 30 2E 30 33 35 6E 20 35 35 31 91 AF
//                 AA 7 FC BF AB 32 32 38 2E 31 B0 B0 B0 B0 B0 6B 33 2E 39 33 37 6D 20 35 35 31 92 AF
    int buf[1024]={0xAA,0x01,0xFC,0xBF,0xAB,0x34,0x39,0x38,0x2E,0x36,0x31,0x38,0x2E,0x38,0x32,0x4D,0x32,0x36,0x2E,0x34,0x38,0x75,0x20,0x35,0x35,0x32,0x91,0xAF,
                   0xAA,0x02,0xFC,0xBF,0xAB,0x34,0x39,0x38,0x2E,0x36,0x34,0x31,0x2E,0x33,0x34,0x4D,0x31,0x32,0x2E,0x30,0x36,0x75,0x20,0x35,0x35,0x32,0x91,0xAF,
                   0xAA,0x03,0xFC,0xBF,0xAB,0x35,0x30,0x31,0x2E,0x33,0x32,0x37,0x2E,0x33,0x36,0x4D,0x31,0x38,0x2E,0x33,0x31,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,
                   0xAA,0x04,0xFC,0xBF,0xAB,0x34,0x39,0x35,0x2E,0x33,0x38,0x2E,0x35,0x35,0x32,0x4D,0x35,0x37,0x2E,0x39,0x31,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,
                   0xAA,0x05,0xFC,0xBF,0xAB,0x34,0x39,0x39,0x2E,0x33,0x38,0x2E,0x35,0x30,0x36,0x4D,0x35,0x38,0x2E,0x37,0x30,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,
                   0xAA,0x06,0xFC,0xBF,0xAB,0x35,0x30,0x38,0x2E,0x30,0x31,0x33,0x2E,0x36,0x38,0x4D,0x33,0x37,0x2E,0x31,0x32,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,
                   0xAA,0x07,0xFC,0xBF,0xAB,0x35,0x30,0x30,0x2E,0x36,0xB0,0xB0,0xB0,0xB0,0xB0,0x4D,0x32,0x31,0x2E,0x32,0x36,0x75,0x20,0x35,0x35,0x30,0x91,0xAF,
                   0xAA,0x08,0xFC,0xBF,0xAB,0x35,0x30,0x31,0x2E,0x33,0xB0,0xB0,0xB0,0xB0,0xB0,0x4D,0x31,0x35,0x2E,0x34,0x34,0x75,0x20,0x35,0x35,0x31,0x92,0xAF,
                   0xCD,0x2C,0x33,0x21,0x66,0xA2,0x00,0x08,0x00,0x08,0x9A,0x29,0x00,0xA4,0x33,0xF3};
    std::vector<std::string> voltage;
    std::vector<std::string> resistance;
    std::vector<bool> testState;
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

    for (int base=0,i=0;i<8;i++) {
        bool flag=false,flag2=false;
        base+=5;
        for (int j=0;j<5;j++) {
            voltage[i]+=buf[base];
            base++;
        }
        voltage[i]+="V";

        if(uint8_t(buf[base])==0xB0){

            flag2=true;
            base+=5;
        }

        else{
            for (int j=0;j<5;j++) {
                resistance[i]+=buf[base];
                base++;
            }
            switch (buf[base]) {
                case 0x47: resistance[i]+="GΩ";break;
                case 0x4D: resistance[i]+="MΩ";break;
                case 0x6B: resistance[i]+="KΩ";flag=true;break;
                case 0x54: resistance[i]+="TΩ";break;
            }
        }
         qDebug()<<"flag2"<<flag2;
        base+=11;
        switch(buf[base]) {
            case 0x91:
                if (flag2){
                    resistance[i]+="Pass";
                 }
                 testState[i]=true;break;
            case 0x92:
                if (flag2){
                    resistance[i]+="0Ω";
                }
                testState[i]=false;break;
        }
        if(flag){
            testState[i]=false;
        }
        base+=2;

    }
//    qDebug()<<"电阻1："<<QString::fromStdString(resistance[0])<<" 电压1："<<QString::fromStdString(voltage[0])<<" 状态1："<<testState[0];
//    qDebug()<<"电阻2："<<QString::fromStdString(resistance[1])<<" 电压2："<<QString::fromStdString(voltage[1])<<" 状态2："<<testState[1];
//    qDebug()<<"电阻3："<<QString::fromStdString(resistance[2])<<" 电压3："<<QString::fromStdString(voltage[2])<<" 状态3："<<testState[2];
//    qDebug()<<"电阻4："<<QString::fromStdString(resistance[3])<<" 电压4："<<QString::fromStdString(voltage[3])<<" 状态4："<<testState[3];
//    qDebug()<<"电阻5："<<QString::fromStdString(resistance[4])<<" 电压5："<<QString::fromStdString(voltage[4])<<" 状态5："<<testState[4];
//    qDebug()<<"电阻6："<<QString::fromStdString(resistance[5])<<" 电压6："<<QString::fromStdString(voltage[5])<<" 状态6："<<testState[5];
//    qDebug()<<"电阻7："<<QString::fromStdString(resistance[6])<<" 电压7："<<QString::fromStdString(voltage[6])<<" 状态7："<<testState[6];
//    qDebug()<<"电阻8："<<QString::fromStdString(resistance[7])<<" 电压8："<<QString::fromStdString(voltage[7])<<" 状态8："<<testState[7];

}

void standarSet::LoadEightPortMes(){
    ui->textEdit->setText(QString::fromStdString(settings.eightChannelFrameMes));
}