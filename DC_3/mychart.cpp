#include "mychart.h"
#include <QtCore/QRandomGenerator>
#include <QValueAxis>
#include <QSplineSeries>

Mychart::Mychart(QGraphicsItem *parent, Qt::WindowFlags wFlags,int type,int linenum,int companytypecode):
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    this->companytypecode=companytypecode;
    //设置图像种类
    this->linenums=linenum;
    chartType=type;
    axisX=new QValueAxis();
//    axisX->setTitleFont(QFont());
    axisX->setTitleText("时间(s)");
    axisY=new QValueAxis();
    axisY->setTitleText("温度(℃)");
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);

    for(int i=0;i<linenums;i++){
        my_series[i]=new QSplineSeries(this);
        pointsNum.push_back(0);
        //设置画笔颜色
        if(type==0){
            my_series[i]->setName(settings.splineName[companytypecode][i]);
            QPen pen(settings.splinePen[i]);
            pen.setWidth(settings.penweight);
            my_series[i]->setPen(pen);
        }else {
            my_series[i]->setName(settings.splineName[companytypecode][i+4]);
            QPen pen(settings.splinePen[i+4]);
            pen.setWidth(settings.penweight);
            my_series[i]->setPen(pen);
        }
        addSeries(my_series[i]);

        my_series[i]->attachAxis(axisX);
        my_series[i]->attachAxis(axisY);

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

void Mychart::changeSplineName(int companytypecode){
//    qDebug()<<"1.1";
    this->companytypecode=companytypecode;
    for (int i=0;i<linenums;i++) {
        my_series[i]->clear();
        pointsNum[i]=0;
    }
    axisX->setRange(-settings.defaultXlen/2,+settings.defaultXlen/2);
    for(int i=0;i<linenums;i++){
//        qDebug()<<linenums;
        if(chartType==0){
            my_series[i]->setName(settings.splineName[companytypecode][i]);
        }else {
            my_series[i]->setName(settings.splineName[companytypecode][i+4]);
        }
    }
}

void Mychart::getMessage(int charttype,int mx,int index,double tcf,double tcs,double tct){
    //type是图标种类，index是曲线下标

    if(charttype==chartType){
        switch (index) {
            case 0:
                my_series[0]->append(mx,tcf);
                my_series[1]->append(mx,tcs);
                pointsNum[0]++;
                pointsNum[1]++;
                if(pointsNum[0]>settings.maxpoints){
                    my_series[0]->remove(0);
                    my_series[1]->remove(0);
                }
            break;
            case 1:
                my_series[2]->append(mx,tcf);
                my_series[3]->append(mx,tcs);
                pointsNum[2]++;
                pointsNum[3]++;
                if(pointsNum[2]>settings.maxpoints){
                    my_series[2]->remove(0);
                    my_series[3]->remove(0);
                }
                axisX->setRange(mx-settings.defaultXlen/2,mx+settings.defaultXlen/2);
            break;
            case 2:
                my_series[0]->append(mx,tcf);
                pointsNum[0]++;
                if(pointsNum[0]>settings.maxpoints){
                    my_series[0]->remove(0);
                }
            break;
            case 3:
                my_series[1]->append(mx,tcf);
                my_series[2]->append(mx,tcs);
                pointsNum[1]++;
                pointsNum[2]++;
                if(pointsNum[1]>settings.maxpoints){
                    my_series[1]->remove(0);
                    my_series[2]->remove(0);
                }
                axisX->setRange(mx-settings.defaultXlen/2,mx+settings.defaultXlen/2);
            break;
            case 4:
                my_series[0]->append(mx,tcf);
                my_series[1]->append(mx,tcs);
                my_series[2]->append(mx,tct);
                pointsNum[0]++;
                pointsNum[1]++;
                pointsNum[2]++;
                if(pointsNum[0]>settings.maxpoints){
                    my_series[0]->remove(0);
                    my_series[1]->remove(0);
                    my_series[2]->remove(0);
                }
                axisX->setRange(mx-settings.defaultXlen/2,mx+settings.defaultXlen/2);
            break;
            default:break;
        }
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
