#include "dbservice.h"
#include "string.h"
#include <QDir>
#include <direct.h>
#include "qcoreapplication.h"
#include <QMessageBox>

class Dbservice db;
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
        qDebug()<<i<<" temperature"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
        a=status[i].size();
        qDebug()<<i<<" status"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
        a=time[i].size();
        qDebug()<<i<<" time"<<"  "<<a;
        if(a<mx&&a!=0){
            mx=a;
        }
        a=timestamp.size();
        qDebug()<<i<<"  timestamp"<<"  "<<a;
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
        libxl::Sheet* sheet = book->addSheet(L"Sheet1");
        if(sheet)
        {
            qDebug()<<"min nums"<<mx;
            for (int line=1,i=0;i<mx;i++,line++) {
                qDebug()<<"error1";
                int col=0;
                sheet->writeStr(line, col++,reinterpret_cast<const wchar_t *>(timestamp[i].utf16()));
                for (int j=0;j<settings.lineNums;j++) {
                    sheet->writeNum(line,col++, temperature[j][i]);
                }
                qDebug()<<"error2";

                for (int j=0;j<settings.lineNums;j++){
                    qDebug()<<status[j][i];
                    sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(status[j][i].utf16()));
                }
                qDebug()<<"error3";
                for (int j=0;j<settings.lineNums;j++){
                    sheet->writeNum(line,col++, time[j][i]);
                }
                qDebug()<<i<<"   "<<col;
                qDebug()<<"error4";
            }
        }
        qDebug()<<"finish!";
        QString filepath=QCoreApplication::applicationDirPath()+"/savefiles/charts/"+date+".xlsx";
        book->save(filepath.toStdWString().c_str());
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
        libxl::Sheet* sheet = book->addSheet(L"Sheet1");
        if(sheet)
        {
            int base=1;
            int size=newDateList.length();
            sheet->writeStr(0,0, QString("时间戳").toStdWString().c_str());
            sheet->writeStr(0,1, reinterpret_cast<const wchar_t *>(QString("公司名称").utf16()));
            sheet->writeStr(0,2, reinterpret_cast<const wchar_t *>(QString("帧种类").utf16()));
            sheet->writeStr(0,3, reinterpret_cast<const wchar_t *>(QString("帧ID").utf16()));
            sheet->writeStr(0,4, reinterpret_cast<const wchar_t *>(QString("数据长度").utf16()));
            sheet->writeStr(0,5, reinterpret_cast<const wchar_t *>(QString("数据内容").utf16()));
            for (auto i=0;i<size;i++,base++) {
                int col=0;
//                qDebug()<<base;
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newDateList[i].utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newCompanyName[i].utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newFrameType[i].utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(QString::number(newFrameID[i],16).utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(QString::number(newFrameLen[i],16).utf16()));
                sheet->writeStr(base,col++, reinterpret_cast<const wchar_t *>(newFrameContent[i].utf16()));
            }
        }
        QString filepath=QCoreApplication::applicationDirPath()+"/savefiles/frames/"+date+".xlsx";
        book->save(filepath.toStdWString().c_str());
        book->release();
    }
    qDebug()<<"表格保存完成.";
}
