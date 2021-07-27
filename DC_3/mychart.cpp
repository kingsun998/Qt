#include "mychart.h"
#include <QtCore/QRandomGenerator>


Mychart::Mychart(QGraphicsItem *parent, Qt::WindowFlags wFlags,int type,int linenum):
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    //设置图像种类
    chartType=type;
    axisX=new QValueAxis();
    axisY=new QValueAxis();
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);

    pointsNum.resize(linenum);

    for(int i=0;i<linenum;i++){
        my_series[i]=new QSplineSeries(this);
        //设置画笔颜色
        if(type==0){
            my_series[i]->setName(settings.splineName[i]);
            QPen pen(settings.splinePen[i]);
            pen.setWidth(settings.penweight);
            my_series[i]->setPen(pen);
        }else {
            my_series[i]->setName(settings.splineName[i+4]);
            QPen pen(settings.splinePen[i+4]);
            pen.setWidth(settings.penweight);
            my_series[i]->setPen(pen);
        }
        addSeries(my_series[i]);

        my_series[i]->attachAxis(axisX);
        my_series[i]->attachAxis(axisY);

        pointsNum[i]=0;

    }

    setAnimationOptions(QChart::NoAnimation);
    axisX->setTickCount(settings.defaultXtickCount);
    axisX->setRange(-settings.defaultXlen/2, settings.defaultXlen/2);
    axisY->setRange(settings.Y_min, settings.Y_max);
    axisY->setTickCount(settings.defaultYtickCount);
//    startTimer(20);
}

Mychart::~Mychart(){

}

void Mychart::getMessage(int charttype,int mx,int index,double tcf,double tcs){
    //type是图标种类，index是曲线下标

    if(charttype==chartType){
        switch (index) {
            case 0:
                my_series[0]->append(mx,tcf);
                my_series[1]->append(mx,tcs);
            break;
            case 1:
                my_series[2]->append(mx,tcf);
                my_series[3]->append(mx,tcs);
                axisX->setRange(mx-settings.defaultXlen/2,mx+settings.defaultXlen/2);
            break;
            case 2:
                my_series[0]->append(mx,tcf);
            break;
            case 3:
                my_series[1]->append(mx,tcf);
                my_series[2]->append(mx,tcs);
                axisX->setRange(mx-settings.defaultXlen/2,mx+settings.defaultXlen/2);
            break;
        default:break;
        }
    }
}

void Mychart::show_hidden_Series(int index,bool selected){
    if(selected==true){
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
