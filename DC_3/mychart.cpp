#include "mychart.h"
#include <QtCore/QRandomGenerator>
#include <QValueAxis>
#include <QSplineSeries>

Mychart::Mychart(QGraphicsItem *parent, Qt::WindowFlags wFlags,int chartType,int linenum,int companytypecode):
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    this->companytypecode=companytypecode;
    //设置图像种类
    this->linenums=linenum;
    this->chartType=chartType;
    axisX=new QValueAxis();
    axisX->setTitleText("时间(s)");
    axisY=new QValueAxis();
    axisY->setTitleText("温度(℃)");
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);

    for(int i=0;i<linenums;i++){
        my_series[i]=new QSplineSeries(this);
        pointsNum.push_back(0);
        //设置画笔颜色
        if(chartType==0){
            my_series[i]->setName(settings.company_usedate_names[i]);
            QPen pen(settings.splinePen[i]);
            pen.setWidth(settings.penweight);
            my_series[i]->setPen(pen);
        }else {
            my_series[i]->setName(settings.company_usedate_names[i+(settings.splinePen.size()+1)/2]);
            QPen pen(settings.splinePen[i+(settings.splinePen.size()+1)/2]);
            pen.setWidth(settings.penweight);
            my_series[i]->setPen(pen);
        }
        addSeries(my_series[i]);

        my_series[i]->attachAxis(axisX);
        my_series[i]->attachAxis(axisY);

    }

    setAnimationOptions(QChart::NoAnimation);
    axisX->setTickCount(settings.defaultXtickCount);
    axisX->setRange(-settings.defaultXlen, 0);
    axisY->setRange(settings.Y_min, settings.Y_max);
    axisY->setTickCount(settings.defaultYtickCount);
//    startTimer(20);
}

Mychart::~Mychart(){

}

void Mychart::changeSplineName(int companytypecode){
//    qDebug()<<"1.1";
    this->companytypecode=companytypecode;
    for (int i=0;i<linenums;i++) {
        my_series[i]->clear();
        pointsNum[i]=0;
    }
    axisX->setRange(-settings.defaultXlen,0);
    for(int i=0;i<linenums;i++){
        if(chartType==0){
            my_series[i]->setName(settings.company_usedate_names[i]);
        }else {
            my_series[i]->setName(settings.company_usedate_names[i+3]);
        }
    }
}

void Mychart::getMessage(int mx,uint index,double val){
     my_series[index]->append(mx,val);
     if(pointsNum[index]<settings.maxpoints){
         pointsNum[index]++;
     }else{
         my_series[index]->remove(0);
     }
     if(index==2){
         axisX->setRange(mx-settings.defaultXlen,mx);
     }
}

void Mychart::show_hidden_Series(int index,bool selected){
    if(selected==false){
        removeSeries(my_series[index]);
    }else{
        addSeries(my_series[index]);
        my_series[index]->attachAxis(axisX);
        my_series[index]->attachAxis(axisY);
    }
}

void Mychart::changeYScale(double y_bt, double y_tp){
    if(abs(y_bt+10000000)<1e-6){
        y_bt=axisY->min();
    }
    if(abs(y_tp+10000000)<1e-6){
        y_tp=axisY->max();
    }
    if(y_bt<y_tp)
        axisY->setRange(y_bt,y_tp);
}
