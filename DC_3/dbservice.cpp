﻿#include "dbservice.h"
#include "memory.h"
Dbservice::Dbservice()
{
    savetable.moveToThread(&tableThread);
    savechart.moveToThread(&chartThread);
    connect(this,&Dbservice::saveChart,&savechart,&saveChart::insert);
    connect(this,&Dbservice::saveTable,&savetable,&saveTable::insert);
    tableThread.start();
    chartThread.start();
}



Dbservice::~Dbservice(){
    tableThread.quit();
    chartThread.quit();
    tableThread.wait();
    chartThread.wait();
}

bool Dbservice::ChartThreadIsAlive(){
    return chartThread.isRunning();
}

bool Dbservice::TableThreadIsAlive(){
    return tableThread.isRunning();
}


void Dbservice::init(){
    qDebug()<<"init";

}

saveChart::saveChart(){

}
saveTable::saveTable(){

}

//wchar_t* toWchart(QString s){
//    return s
//}
void saveChart::insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date){
    qDebug()<<"子线程"<<QThread::currentThreadId();
    qDebug()<<"子线程时间"<<QDateTime::currentDateTime().toUTC();
    QVector<QVector<double>> temperature=tp.value<QVector<QVector<double>>>();
    QVector<QString> timestamp=ts.value<QVector<QString>>();
    QVector<QVector<QString>> status=sta.value<QVector<QVector<QString>>>();
    QVector<QVector<double>> time=tm.value<QVector<QVector<double>>>();


    int mx=1e+9,a;

    for(int i=0;i<settings.totalnums;i++){
        a=temperature[i].size();
//        qDebug()<<i<<" temperature"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
        a=status[i].size();
//        qDebug()<<i<<" status"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
        a=time[i].size();
//        qDebug()<<i<<" time"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
        a=timestamp.size();
//        qDebug()<<i<<"  timestamp"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
    }
    if(mx==1e+9){
        return;
    }
    qDebug()<<"error1";
    libxl::Book* book=xlCreateBookW();
    if(book)
    {
        //Qstring 转换为 wchart
        libxl::Sheet* sheet = book->addSheet(L"1");
        if(sheet)
        {
//            qDebug()<<"min nums"<<mx;
            for (int line=1,i=0;i<mx;i++,line++) {

                int col=0;
                sheet->writeStr(line, col++,reinterpret_cast<const wchar_t *>(timestamp[i].utf16()));
                for (int j=0;j<settings.lineNums;j++) {
                    sheet->writeNum(line,col++, temperature[j][i]);
                }
                for (int j=0;j<settings.lineNums;j++){
                    sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(status[j][i].utf16()));
                }
                for (int j=0;j<settings.lineNums;j++){
                    sheet->writeNum(line,col++, time[j][i]);
                }
                qDebug()<<i<<"   "<<col;
            }
        }
        qDebug()<<"finish!";
        book->save(reinterpret_cast<const wchar_t *>((QString("../savefiles/Chart/")+date+QString(".xlsx")).utf16()));
        book->release();
    }
    qDebug()<<"图表保存完成.";

}

//content,data,id,type,name,len,
void saveTable::insert(QVariant content,QVariant timestamp,QVariant id,
                       QVariant type,QVariant name,QVariant len,QString date){
    QVector<QString> newFrameType=type.value<QVector<QString>>();
    QVector<QString> newDateList=timestamp.value<QVector<QString>>();
    qDebug()<<"数组s大小 "<<newDateList.size();

    QVector<QString> newCompanyName=name.value<QVector<QString>>();
    QVector<uint>  newFrameLen=len.value<QVector<uint>>();
    qDebug()<<"数组s大小 "<<newFrameLen.size();
    QVector<QString> newFrameContent=content.value<QVector<QString>>();
    QVector<uint> newFrameID=id.value<QVector<uint>>();
    libxl::Book* book=xlCreateBookW();
    if(book)
    {
        //Qstring 转换为 wchart
        libxl::Sheet* sheet = book->addSheet(L"1");
        if(sheet)
        {
            int base=1;
            int size=newDateList.length();
            qDebug()<<"数组大小 "<<size;
            sheet->writeStr(0,0, reinterpret_cast<const wchar_t *>(QString("时间戳").utf16()));
            sheet->writeStr(0,1, reinterpret_cast<const wchar_t *>(QString("公司名称").utf16()));
            sheet->writeStr(0,2, reinterpret_cast<const wchar_t *>(QString("帧种类").utf16()));
            sheet->writeStr(0,3, reinterpret_cast<const wchar_t *>(QString("帧ID").utf16()));
            sheet->writeStr(0,4, reinterpret_cast<const wchar_t *>(QString("数据长度").utf16()));
            sheet->writeStr(0,5, reinterpret_cast<const wchar_t *>(QString("数据内容").utf16()));
            for (auto i=0;i<size;i++,base++) {
                int col=0;
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newDateList[i].utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newCompanyName[i].utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newFrameType[i].utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(QString::number(newFrameID[i],16).utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(QString::number(newFrameLen[i],16).utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newFrameContent[i].utf16()));
            }
        }
        qDebug()<<"finish!";
        qDebug()<<QString("../savefiles/Frame/")+date+QString(".xlsx");
        book->save(reinterpret_cast<const wchar_t *>((QString("../savefiles/Frame/")+date+QString(".xlsx")).utf16()));
        book->release();
    }
    qDebug()<<"表格保存完成.";
}
