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
    QVector<QVector<QString>> timestamp=ts.value<QVector<QVector<QString>>>();
    QVector<QVector<QString>> status=sta.value<QVector<QVector<QString>>>();
    QVector<QVector<double>> time=tm.value<QVector<QVector<double>>>();
    qDebug()<<QString("../savefiles/"+date+".txt");

    int mx=1e+9,a,b,c,d;
    for(int i=0;i<7;i++){
        a=temperature[i].size();
        if(a<mx){
            mx=a;
        }
        b=timestamp[i].size();
        if(b<mx){
            mx=b;
        }
        c=timestamp[i].size();
        if(c<mx){
            mx=c;
        }
        d=timestamp[i].size();
        if(d<mx){
            mx=d;
        }
    }
    if(mx==1e+9){
        return;
    }
    qDebug()<<"error1";
    libxl::Book* book=xlCreateBook();
    if(book)
    {
        //Qstring 转换为 wchart
        libxl::Sheet* sheet = book->addSheet(L"1");
        if(sheet)
        {
            for (int i=0;i<mx;i++) {
                for (int j=0;j<7;j++) {
                    int index=i*7+j+1;
                    qDebug()<<index;
                    sheet->writeNum(index, 0, j+1);
                    sheet->writeNum(index, 1, temperature[j][i]);
                    sheet->writeStr(index, 2,reinterpret_cast<const wchar_t *>(status[j][i].utf16()));
                    sheet->writeNum(index, 3, time[j][i]);
                    sheet->writeStr(index, 4, reinterpret_cast<const wchar_t *>(timestamp[j][i].utf16()));
                }

            }
        }

        book->save(reinterpret_cast<const wchar_t *>((QString("../savefiles/")+date+QString(".xlsx")).utf16()));
        book->release();
    }
    qDebug()<<"保存完成.";

}
void saveTable::insert(QVariant tp,QVariant ts,QVariant sta,QVariant tm,QString date){

}
