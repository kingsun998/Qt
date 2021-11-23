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
    current_page=-1;
}
saveTable::saveTable(){
    current_page=-1;
}


void saveChart::createXlsx(QString filename){
    libxl::Book* book=xlCreateBookW();
    //  创建新的excel
    this->current_page=0;
    createSheet(book);
    book->save(filename.toStdWString().c_str());
    book->release();
}

void saveChart::createSheet(libxl::Book *book){
    //  创建新的excel
    // 默认为-1，这里变成0
    libxl::Sheet* sheet = book->addSheet(QString::number(this->current_page).toStdWString().c_str());

    sheet->writeStr(0,0, QString("时间戳").toStdWString().c_str());

    int colnums=1;
    for(int i=0;i<settings.lineNums;i++){
        sheet->writeStr(0,colnums,QString("温度_"+settings.splineName[settings.CompanyType][i]).toStdWString().c_str());
        colnums++;
    }
    for(int i=0;i<settings.lineNums;i++){
        sheet->writeStr(0,colnums,QString("状态_"+settings.splineName[settings.CompanyType][i]).toStdWString().c_str());
        colnums++;
    }

    for(int i=0;i<settings.lineNums;i++){
        sheet->writeStr(0,colnums,QString("上升时间_"+settings.splineName[settings.CompanyType][i]).toStdWString().c_str());
        colnums++;
    }

}

void saveChart::insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date){

    QVector<QVector<double>> temperature=tp.value<QVector<QVector<double>>>();
    QVector<QString> timestamp=ts.value<QVector<QString>>();
    QVector<QVector<QString>> status=sta.value<QVector<QVector<QString>>>();
    QVector<QVector<double>> time=tm.value<QVector<QVector<double>>>();

    QString filename=QCoreApplication::applicationDirPath()+"/savefiles/charts/"+date+
            "_"+settings.CompanyName[settings.CompanyType]+".xls";

    if(!QFile::exists(filename)){
        //不存在
        first_page=true;
        createXlsx(filename);
    }
//    showMessage(" 到这里 ",true);
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

    libxl::Book* book=xlCreateBookW();

    if(book)
    {
        book->load(filename.toStdWString().c_str());
//        showMessage("到这里1.5",true);
        //Qstring 转换为 wchart
        this->current_page=book->sheetCount()-1;
        libxl::Sheet* sheet = book->getSheet(current_page);
//        showMessage("到这里2",true);
        long base=sheet->lastRow();
        if(first_page){
            base+=1;
            first_page=false;
        }
//        showMessage("到这里2.5",true);
        if(sheet)
        {
            for (long line=base,i=0;i<mx;i++,line++) {
//                showMessage(QString::number(line),true);
                if(line>settings.xlsxMaxline){
                    this->current_page++;
                    createSheet(book);
                    sheet = book->getSheet(current_page);
                    line=sheet->lastRow();
                }
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

        book->save(filename.toStdWString().c_str());
        book->release();
    }
    qDebug()<<"图表保存完成.";
}

void saveTable::createXlsx(QString filename){
    libxl::Book* book=xlCreateBookW();
    current_page=0;
    //  创建新的excel
    createSheet(book);
    book->save(filename.toStdWString().c_str());
    book->release();
}

void saveTable::createSheet(libxl::Book *book){
    //  创建新的excel

    libxl::Sheet* sheet = book->addSheet(QString::number(this->current_page).toStdWString().c_str());
    sheet->writeStr(0,0, QString("时间戳").toStdWString().c_str());
    sheet->writeStr(0,1, QString("发送/接受").toStdWString().c_str());
    sheet->writeStr(0,2, reinterpret_cast<const wchar_t *>(QString("公司名称").utf16()));
    sheet->writeStr(0,3, reinterpret_cast<const wchar_t *>(QString("帧种类").utf16()));
    sheet->writeStr(0,4, reinterpret_cast<const wchar_t *>(QString("帧ID").utf16()));
    sheet->writeStr(0,5, reinterpret_cast<const wchar_t *>(QString("数据长度").utf16()));
    sheet->writeStr(0,6, reinterpret_cast<const wchar_t *>(QString("数据内容").utf16()));

}

//content,data,id,type,name,len,
void saveTable::insert(QVariant content,QVariant timestamp,QVariant id,
                       QVariant type,QVariant name,QVariant len,QVariant ifsend,QString date){
    QVector<QString> newFrameType=type.value<QVector<QString>>();
    QVector<QString> newDateList=timestamp.value<QVector<QString>>();
//    qDebug()<<"数组s大小 "<<newDateList.size();
    QVector<bool> newIfsend=ifsend.value<QVector<bool>>();
    QVector<QString> newCompanyName=name.value<QVector<QString>>();
    QVector<uint>  newFrameLen=len.value<QVector<uint>>();
//    qDebug()<<"数组s大小 "<<newFrameLen.size();
    QVector<QString> newFrameContent=content.value<QVector<QString>>();
    QVector<uint> newFrameID=id.value<QVector<uint>>();


    QString filename=QCoreApplication::applicationDirPath()+"/savefiles/frames/"+date+".xls";
    if(!QFile::exists(filename)){
        //不存在
        first_page=true;
        createXlsx(filename);
    }
    libxl::Book* book=xlCreateBookW();
    if(book)
    {
        book->load(filename.toStdWString().c_str());
        //Qstring 转换为 wchart
        this->current_page=book->sheetCount()-1;
        libxl::Sheet* sheet = book->getSheet(current_page);
        int base=sheet->lastRow();
        if(first_page){
            base+=1;
            first_page=false;
        }
        if(sheet)
        {
            int size=newDateList.length();
            for (long i=0,line=base;i<size;i++,line++) {
                if(line>settings.xlsxMaxline){
                    this->current_page++;
                    createSheet(book);
                    sheet = book->getSheet(current_page);
                    line=sheet->lastRow();
                }
                int col=0;
                sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(newDateList[i].utf16()));\
                if(newIfsend[i]){
                    sheet->writeStr(line,col++, QString("发送帧").toStdWString().c_str());
                }
                else{
                    sheet->writeStr(line,col++, QString("接受帧").toStdWString().c_str());
                }
                sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(newCompanyName[i].utf16()));
                sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(newFrameType[i].utf16()));
                sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(QString::number(newFrameID[i],16).utf16()));
                sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(QString::number(newFrameLen[i],16).utf16()));
                sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(newFrameContent[i].utf16()));
            }
        }
        book->save(filename.toStdWString().c_str());
        book->release();
    }
    qDebug()<<"表格保存完成.";
}
