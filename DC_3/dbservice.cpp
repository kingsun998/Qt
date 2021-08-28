#include "dbservice.h"
#include "string.h"
#include <QDir>
#include <direct.h>
#include "qcoreapplication.h"
#include <QMessageBox>
#include <QDateTime>
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
//    qDebug()<<"init";

}

saveChart::saveChart(){

}
saveTable::saveTable(){

}

//wchar_t* toWchart(QString s){
//    return s
//}
void saveChart::insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date){
//    qDebug()<<"子线程"<<QThread::currentThreadId();
//    qDebug()<<"子线程时间"<<QDateTime::currentDateTime().toUTC();
    QVector<QVector<double>> temperature=tp.value<QVector<QVector<double>>>();
    QVector<QString> timestamp=ts.value<QVector<QString>>();
    QVector<QVector<QString>> status=sta.value<QVector<QVector<QString>>>();
    QVector<QVector<double>> time=tm.value<QVector<QVector<double>>>();

    QString filename=QCoreApplication::applicationDirPath()+"/savefiles/charts/"+date+".xlsx";
    libxl::Book* book=nullptr;

    if(!QFile::exists(filename)){
        //不存在
        book=xlCreateBookW();
        libxl::Sheet* sheet = book->addSheet(L"Sheet1");
        book->save(filename.toStdWString().c_str());
        book->release();
    }

    int mx=1e+9,a;
    for(int i=0;i<settings.totalnums;i++){
        a=temperature[i].size();
        if(a<mx&&a!=0){
            mx=a;
        }
        a=status[i].size();
        if(a<mx&&a!=0){
            mx=a;
        }
        a=time[i].size();
        if(a<mx&&a!=0){
            mx=a;
        }
        a=timestamp.size();
        if(a<mx&&a!=0){
            mx=a;
        }
    }
    if(mx==1e+9){
        return;
    }

    book=xlCreateBookW();
    if(book)
    {
//        qDebug()<<"error1";
        book->load(filename.toStdWString().c_str());
//        qDebug()<<"sheet count"<<book->sheetCount();
        //Qstring 转换为 wchart
        libxl::Sheet* sheet = book->getSheet(0);
        int base=sheet->lastRow();
//        qDebug()<<"last "<<base;
        if(sheet)
        {
//            qDebug()<<"min nums"<<mx;
            for (int line=base,i=0;i<mx;i++,line++) {
                int col=0;

                sheet->writeStr(line, col++,timestamp[i].toStdWString().c_str());
                for (int j=0;j<settings.lineNums;j++) {
                    sheet->writeNum(line,col++, temperature[j][i]);
                }
                for (int j=0;j<settings.lineNums;j++){
                    sheet->writeStr(line,col++,status[j][i].toStdWString().c_str());
                }
                for (int j=0;j<settings.lineNums;j++){
                    sheet->writeNum(line,col++, time[j][i]);
                }
            }
        }
//        qDebug()<<"finish!";
        book->save(filename.toStdWString().c_str());
        book->release();
    }
//    qDebug()<<"图表保存完成.";
}

//content,data,id,type,name,len,
void saveTable::insert(QVariant content,QVariant timestamp,QVariant id,
                       QVariant type,QVariant name,QVariant len,QString date){
    QVector<QString> newFrameType=type.value<QVector<QString>>();
    QVector<QString> newDateList=timestamp.value<QVector<QString>>();
//    qDebug()<<"数组s大小 "<<newDateList.size();

    QVector<QString> newCompanyName=name.value<QVector<QString>>();
    QVector<uint>  newFrameLen=len.value<QVector<uint>>();
//    qDebug()<<"数组s大小 "<<newFrameLen.size();
    QVector<QString> newFrameContent=content.value<QVector<QString>>();
    QVector<uint> newFrameID=id.value<QVector<uint>>();
    libxl::Book* book=nullptr;

    QString filename=QCoreApplication::applicationDirPath()+"/savefiles/frames/"+date+".xlsx";
    if(!QFile::exists(filename)){
        //不存在
        book=xlCreateBookW();

        libxl::Sheet* sheet = book->addSheet(L"Sheet1");
        sheet->writeStr(0,0, QString("时间戳").toStdWString().c_str());
        sheet->writeStr(0,1, reinterpret_cast<const wchar_t *>(QString("公司名称").utf16()));
        sheet->writeStr(0,2, reinterpret_cast<const wchar_t *>(QString("帧种类").utf16()));
        sheet->writeStr(0,3, reinterpret_cast<const wchar_t *>(QString("帧ID").utf16()));
        sheet->writeStr(0,4, reinterpret_cast<const wchar_t *>(QString("数据长度").utf16()));
        sheet->writeStr(0,5, reinterpret_cast<const wchar_t *>(QString("数据内容").utf16()));
        book->save(filename.toStdWString().c_str());
        book->release();
    }
    book=xlCreateBookW();
    if(book)
    {
        book->load(filename.toStdWString().c_str());
        //Qstring 转换为 wchart
        libxl::Sheet* sheet = book->getSheet(0);
        int base=sheet->lastRow();
//        qDebug()<<"Base "<<base;
        if(sheet)
        {
            int size=newDateList.length();
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
        book->save(filename.toStdWString().c_str());
        book->release();
    }
//    qDebug()<<"表格保存完成.";
}
