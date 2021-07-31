#include "dbservice.h"
#include "string"
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


//void Dbservice::saveChart(QVector<double> tp[7],QVector<QString> timestamp[7],
//                            QVector<QString> status[7],QVector<double> time[7],QString date){
//    savechart->insert(tp,timestamp,status,time,date);
//}


//void Dbservice::saveTable(QVector<double> tp[7],QVector<QString> timestamp[7],
//                            QVector<QString> status[7],QVector<double> time[7],QString date){
//    savechart->insert(tp,timestamp,status,time,date);
//}

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
    qDebug()<<QString("../savefiles/"+date+".txt");

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
//    libxl::Book* book=xlCreateBookW();
//    if(book)
//    {
//        //Qstring 转换为 wchart
//        libxl::Sheet* sheet = book->addSheet(L"1");
//        if(sheet)
//        {
//            qDebug()<<"min nums"<<mx;
//            for (int line=1,i=0;i<mx;i++,line++) {
//                int col=0;
//                sheet->writeStr(line, col++, reinterpret_cast<const wchar_t *>(timestamp[i].utf16()));
//                for (int j=0;j<settings.lineNums;j++) {
//                    sheet->writeNum(line,col++, temperature[j][i]);
//                }
//                for (int j=0;j<settings.lineNums;j++){
//                    sheet->writeStr(line,col++, reinterpret_cast<const wchar_t *>(status[j][i].utf16()));
//                }
//                for (int j=0;j<settings.lineNums;j++){
//                    sheet->writeNum(line,col++, time[j][i]);
//                }
//                qDebug()<<i<<"   "<<col;
//            }
//        }
//        qDebug()<<"finish!";
//        book->save(reinterpret_cast<const wchar_t *>((QString("../savefiles/")+date+QString(".xlsx")).utf16()));
//        book->release();
//    }
    qDebug()<<"保存完成.";

}
void saveTable::insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date){

}
