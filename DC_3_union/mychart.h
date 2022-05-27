﻿#ifndef MYCHART_H
#define MYCHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <settings.h>
#include <vector>
QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Mychart : public QChart
{
public:
    Mychart(QGraphicsItem *parent, Qt::WindowFlags wFlags,int chartType,int linenum,int companytypecode);
    ~Mychart();
    void changeYScale(double y_bt=-10000000,double y_tp=-10000000);
public slots:
    void getMessage(int mx,uint index,double val);
    void show_hidden_Series(int index,bool selected);
    void changeSplineName(int companytypecode);
private:
    int chartType;
    QSplineSeries * my_series[8];
    QValueAxis *axisX;
    QValueAxis *axisY;
    int companytypecode;
    int linenums;
//    QVector<QList<QPointF> *> points;
    std::vector<int> pointsNum;
};

#endif // MYCHART_H
